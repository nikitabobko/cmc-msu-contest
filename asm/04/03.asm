%include 'io.inc'

section .text
global CMAIN
CMAIN:
    mov ebp, esp
    call F
    xor eax, eax
    ret
    
F:
    push ebp
    mov ebp, esp
    GET_UDEC 4, eax
    cmp eax, 0
    jne .else
        pop ebp
        ret
    .else:
    push eax
    call F
    PRINT_DEC 4, [ebp-4]
    PRINT_CHAR ' '
    mov esp, ebp
    pop ebp
    ret