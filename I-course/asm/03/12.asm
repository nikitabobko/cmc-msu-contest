%include 'io.inc'

section .bss
    triangle resw 406 ; 1+2+...+28

section .text
global CMAIN
global combination
CMAIN:
    mov ebp, esp
    
    ; fill Pascal's triangle
    mov esi, 2
.nloop:
    mov edi, 1
.kloop:
    ; ebx = combination(n-1, k)
    dec esi
    push edi
    push esi
    call combination
    add esp, 8
    mov ebx, eax

    ; eax = combination(n-1, k-1)
    dec edi
    push edi
    push esi
    call combination
    add esp, 8

    add ebx, eax

    ; return esi and edi their old values
    inc edi
    inc esi

    ; eax = (n-2)(n-1)/2 + k - 1
    mov eax, esi
    sub eax, 2
    mov ecx, esi
    sub ecx, 1
    mul ecx
    shr eax, 1
    add eax, edi
    dec eax

    mov [triangle+4*eax], ebx

    inc edi
    cmp edi, esi
    jb .kloop 

    inc esi
    cmp esi, 30
    jb .nloop

    GET_UDEC 4, ebx
    GET_UDEC 4, esi

    mov eax, ebx    
    mov ecx, 32
.loop:
    shr eax, 1
    loopnz .loop

    ; edi = n-1, n - number of bits in number
    mov edi, 32
    sub edi, ecx 
    dec edi

    ; eax = combination(n-1, k+1) = combination(n-1, k) + 
    ; combination(n-2,k) + ... + combination(n-(n-k), k)
    inc  esi
    push esi
    dec  esi
    push edi
    call combination
    add esp, 8

    ; [ebp-4] = eax = combination(n-1, k+1)
    push eax

    xor eax, eax

    mov ecx, edi
    jecxz .endloop1
.loop1:
    mov edx, ebx

    dec ecx
    shr edx, cl
    inc ecx

    ; if(edx != 0)
    and edx, 0x1
    jz .else 
    push eax
    push ecx

    ; eax = combination(ecx-1, esi-eax-1), esi = k, eax = number of zeros
    dec esi
    sub esi, eax
    push esi
    add esi, eax
    inc esi
    dec ecx
    push ecx
    inc ecx
    call combination
    add esp, 8
    
    add eax, [ebp-4]
    mov [ebp-4], eax

    pop ecx
    pop eax
    jmp .endelse
.else:
    inc eax
.endelse:
    loop .loop1  
.endloop1:

    ; edx = almost answer
    mov edx, [ebp-4]

    cmp eax, esi
    jne .else1
    inc edx
.else1:

    PRINT_DEC 4, edx
            
    mov esp, ebp
    xor eax, eax
    ret
    
; int combination(int n, int k) = n!/(k!(n-k)!)
combination:
    mov eax, [esp+4]

    cmp eax, [esp+8]
    jb .zero
    jne .else
    mov eax, 1
    ret
.zero:
    xor eax, eax
    ret
.else:
    mov ecx, [esp+8]
    test ecx, ecx
    jnz .else1
    mov eax, 1
    ret
.else1:

    sub eax, 2

    mov ecx, [esp+4]
    sub ecx, 1

    xor edx, edx
    mul ecx

    shr eax, 1

    add eax, [esp+8]
    dec eax

    mov eax, [triangle+4*eax]
    ret
    