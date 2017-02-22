%include "io.inc"

section .bss
    x1 resd 1
    y1 resd 1
    x2 resd 1
    y2 resd 1
    x3 resd 1
    y3 resd 1

section .text
global CMAIN
CMAIN:
    mov ebp, esp; for correct debugging
    GET_DEC 4, x1
    GET_DEC 4, y1
    GET_DEC 4, x2
    GET_DEC 4, y2
    GET_DEC 4, x3
    GET_DEC 4, y3
    
    mov eax, [x2]
    sub eax, [x1]
    mov ecx, [y3]
    sub ecx, [y1]
    mul ecx
    
    mov ecx, [y2]
    sub ecx, [y1]
    mov edx, [x3]
    sub edx, [x1]
    imul ecx, edx
    
    sub eax, ecx
    
    cdq
    xor eax, edx
    sub eax, edx
    
    mov ecx, eax
    and ecx, 0x1
    imul ecx, 5
    shr eax, 1
    
    PRINT_DEC 4, eax
    PRINT_CHAR "."
    PRINT_DEC 4, ecx    
    
    xor eax, eax
    ret