%include "io.inc"

section .text
global CMAIN
	
CMAIN:
    mov ebp, esp; for correct debugging
    GET_CHAR eax
    ; eax = 'H' - eax
    imul eax, -1
    add eax, 'H'

    GET_DEC 4, ebx
    ; ebx = 8 - ebx
    imul ebx, -1
    add ebx, 8
    ; eax *= ebx
    mul ebx
    ; eax /= 2
    sar eax, 1
    
    PRINT_DEC 4, eax
    NEWLINE
    xor eax, eax
    ret
