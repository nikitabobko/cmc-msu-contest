%include "io.inc"

section .text
global CMAIN
CMAIN:
    mov ebp, esp; for correct debugging
    GET_UDEC 4, eax
    GET_UDEC 4, ebx
    mul ebx
    mov ecx, edx
    GET_UDEC 4, ebx
    mul ebx
    imul ecx, ebx
    add edx, ecx
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
    sub ebx, 360
    
    sar ebx, 0x1f
    and ebx, 0x1
    xor ebx, 0x1
    
    mov ecx, eax
    
    sub eax, 1
    mov esi, 3
    xor edx, edx
    div esi
    add eax, 1
    mul ebx
    sub ecx, eax
    
    PRINT_DEC 4, ecx
    
    xor eax, eax
    ret