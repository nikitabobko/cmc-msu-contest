%include "io.inc"

section .text
global CMAIN
CMAIN:
    mov ebp, esp; for correct debugging
    GET_UDEC 4, eax
    xor ebx, ebx
    xor ecx, ecx
    GET_UDEC 1, bl
    mov cl, 32
    sub cl, bl
    
    shl eax, cl
    shr eax, cl
    
    PRINT_UDEC 4, eax
    xor eax, eax
    ret
    