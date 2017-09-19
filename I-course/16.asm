%include "io.inc"

section .bss
    a11 resd 1
    a12 resd 1
    a21 resd 1
    a22 resd 1
    b1  resd 1
    b2  resd 1

section .text
global CMAIN
CMAIN:
    mov ebp, esp; for correct debugging
    GET_UDEC 4, a11
    GET_UDEC 4, a12
    GET_UDEC 4, a21
    GET_UDEC 4, a22
    
    ; eax = not ((a11 and a22) xor (a12 and a21))
    mov eax, [a11]
    and eax, [a22]
    mov ebx, [a12]
    and ebx, [a21]
    xor eax, ebx
    not eax
    
    GET_UDEC 4, b1
    GET_UDEC 4, b2

    ; X CALCULATIONS
    
    ; ebx = (b1 and a22) xor (a12 and b2)
    mov ebx, [b1]
    and ebx, [a22]
    mov ecx, [a12]
    and ecx, [b2]
    xor ebx, ecx

    ; edx = (a11 or a21) and (b1 or b2)
    mov edx, [a11]
    or  edx, [a21]
    mov ecx, [b1]
    or  ecx, [b2]
    and edx, ecx
    
    ; edx &= eax
    ; ebx = ebx xor edx
    and edx, eax
    xor ebx, edx

    PRINT_UDEC 4, ebx
    NEWLINE

    ; Y CALCULATIONS
    
    ; ebx = (a11 and b2) xor (a21 and b1)
    mov ebx, [a11]
    and ebx, [b2]
    mov ecx, [a21]
    and ecx, [b1]
    xor ebx, ecx

    ; edx = (not (a11 or a21)) and (a12 or a22) and (b1 or b2)
    mov edx, [a11]
    or  edx, [a21]
    not edx
    mov ecx, [a12]
    or  ecx, [a22]
    and edx, ecx
    mov ecx, [b1]
    or  ecx, [b2]
    and edx, ecx
    
    ; edx &= eax
    ; ebx = ebx xor edx
    and edx, eax
    xor ebx, edx

    PRINT_UDEC 4, ebx
    NEWLINE

    xor eax, eax
    ret
    