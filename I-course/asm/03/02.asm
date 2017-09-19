%include 'io.inc'

section .text
global CMAIN
CMAIN:
    mov ebp, esp; for correct debugging
    GET_UDEC 4, eax
    mov ecx, 0
.loop:
    test eax, eax
    jz .end 

    mov ebx, eax
    and ebx, 0x1
    add ecx, ebx
    
    shr eax, 1
    
    jmp .loop
.end:

    PRINT_DEC 4, ecx    
    xor eax, eax
    ret