%include "io.inc"

section .text
global CMAIN
CMAIN:
    GET_UDEC 4, eax
    GET_UDEC 4, ebx
    mul ebx
    GET_UDEC 4, ebx
    mul ebx
    GET_UDEC 4, ebx
    xor edx, edx
    div ebx
    imul edx, -1
    sar edx, 0x1f
    imul edx, -1
    add eax, edx
    
    GET_UDEC 4, ebx
    GET_UDEC 4, ecx
    imul ebx, 60
    add ebx, ecx
    sub ebx, 359
    
    sar ebx, 0x1f
    and ebx, 0x1
    xor ebx, 0x1
    
    mov ecx, eax
    
    mov esi, 3
    div esi
    add eax, 1
    mul ebx
    
    sub ecx, eax
    
    PRINT_DEC 4, ecx
    
    xor eax, eax
    ret