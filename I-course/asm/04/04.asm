%include 'io.inc'

section .text
global CMAIN
global F
CMAIN:
    mov ebp, esp; for correct debugging
    xor ebx, ebx
    call F
    ret
    
F:
    GET_DEC 4, eax
    cmp eax, 0
    jnz .endif1
        ret
    .endif1:
    not ebx
    ; if (ebx != 0)
    test ebx, ebx
    jnz .endif 
        push eax
        call F
        pop eax
    .endif:
    PRINT_DEC 4, eax
    PRINT_CHAR ' '
    call F
    ret