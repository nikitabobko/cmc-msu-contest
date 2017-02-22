%include "io.inc"

section .text
global CMAIN
CMAIN:
    ; d = (c & a) + (-c & b)
    GET_HEX 4, eax
    GET_HEX 4, ebx
    GET_HEX 4, ecx
    
    and eax, ecx
    not ecx
    and ebx, ecx
    or  eax, ebx
    
    PRINT_HEX 4, eax
    xor eax, eax
    ret