%include 'io.inc'

section .text
global CMAIN
CMAIN:
    mov ebp, esp
    call F
    xor eax, eax
    ret
    
F:
    GET_UDEC 4, eax
    cmp eax, 0
    jne .else
        ret
    .else:
    push eax
    call F
    pop eax
    PRINT_DEC 4, eax
    PRINT_CHAR ' '
    ret