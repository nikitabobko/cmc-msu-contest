%include 'io.inc'

section .text
global CMAIN
CMAIN:
    mov ebp, esp
    
    ret
    
    
global combination
; int combination(int n, int k) = n!/(k!(n-k)!)
combination:
    push ebp
    mov ebp, esp
    mov edx, [ebp+8]
    
    cmp edx, [esp+12]
    jb .exit

    mov edx, [ebp+8]
    sub edx, [ebp+12]
    mov eax, 0x1
    mov ecx, edx
    sub ecx, 2
    js .endloop1
.loop0:
    mul edx
    dec edx
    loop .loop1
.endloop0:
    ; [ebp+4] = (n-k)!
    push eax
    
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
    
    ; ecx = (n-k)!
    pop ecx
    xor edx, edx
    div ecx

    ret
    
.exit:
    xor eax, eax
    ret