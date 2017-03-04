%include 'io.inc'

section .text
global CMAIN
global combination
CMAIN:
    mov ebp, esp
    
    GET_UDEC 4, ebx
    GET_UDEC 4, esi

    mov eax, ebx    
    mov ecx, 0x20
.loop:
    shr eax, 1
    loopnz .loop
    
    ; edi = number of bits in number
    mov edi, 0x20
    sub edi, ecx 
    
    ; eax = combination(n-1, k+1) = combination(n-1, k) + 
    ; combination(n-2,k) + ... + combination(n-(n-k), k)
    inc  esi
    push esi
    dec  esi
    dec  edi
    push edi
    inc  edi
    call combination
    
    mov ecx, eax
    
    
    mov esp, ebp
    xor eax, eax
    ret
    
; int combination(int n, int k) = n!/(k!(n-k)!)
combination:
    push ebp
    mov ebp, esp
    mov edx, [ebp+8]
    
    cmp edx, [ebp+12]
    jb .exit

    mov edx, [ebp+8]
    sub edx, [ebp+12]
    mov eax, 0x1
    mov ecx, edx
    sub ecx, 2
    js .endloop0
.loop0:
    mul edx
    dec edx
    loop .loop0
.endloop0:
    ; [ebp+4] = (n-k)!
    push eax
    
    ; eax = n!/k!
    mov edx, [ebp+8]
    mov ecx, edx
    sub ecx, [ebp+12]
    mov eax, 0x1
    js .endloop
.loop:
    mul edx
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