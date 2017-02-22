%include "io.inc"

section .text
global CMAIN
CMAIN:
    mov cl, 0x8
    xor eax, eax
    xor ebx, ebx

    GET_UDEC 1, bl
    
    GET_UDEC 1, al
    shl eax, cl
    or  ebx, eax
    xor eax, eax
    
    add cl, 0x8 
    GET_UDEC 1, al
    shl eax, cl
    or ebx, eax
    xor eax, eax
    
    add cl, 0x8
    GET_UDEC 1, al
    shl eax, cl
    or ebx, eax

    PRINT_UDEC 4, ebx

    xor eax, eax    
    ret
    