%include 'io.inc'

section .text
global CMAIN
global combination
CMAIN:
    mov ebp, esp
    
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
    push ebp
    mov ebp, esp
    mov edx, [ebp+8]
    
    cmp edx, [ebp+12]
    jl .exit

    ; eax = (n-k)!
    sub edx, [ebp+12]
    mov eax, 0x1
    mov ecx, edx
    dec ecx
    js .endloop0
    jecxz .endloop0
.loop0:
    push ecx
    push edx
    mov ecx, edx
    xor edx, edx
    mul ecx
    pop edx
    pop ecx
    dec edx
    loop .loop0
.endloop0:

    ; [ebp+4] = (n-k)!
    push eax
    
    ; eax = n!/k!
    mov edx, [ebp+8]
    mov eax, 0x1
    mov ecx, edx
    sub ecx, [ebp+12]
    js .endloop
    jecxz .endloop
.loop:
    push edx
    push ecx
    mov ecx, edx
    xor edx, edx
    mul ecx
    pop ecx
    pop edx
    dec edx
    loop .loop
.endloop:
    
    pop ecx ; ecx = (n-k)!
    xor edx, edx
    div ecx

    pop ebp
    ret
    
.exit:
    pop ebp
    xor eax, eax
    ret