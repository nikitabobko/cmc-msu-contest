%include 'io.inc'

section .text
global CMAIN
CMAIN:
    mov ebp, esp
    GET_UDEC 4, ecx
    dec ecx
    mov edx, 1
    mov esi, 1
    GET_DEC 4, ebx
    jecxz .endloop
    .loop:
        GET_DEC 4, eax
        cmp eax, ebx
        ; if (eax > ebx)
        jng .else 
            inc edx
            jmp .endif
        .else:
            cmp edx, esi
            jng .endif1
                mov esi, edx
            .endif1:
            mov edx, 0x1
        .endif:
        mov ebx, eax
        loop .loop
    .endloop:
    cmp edx, esi
    cmovg esi, edx
    PRINT_UDEC 4, esi
    mov esp, ebp
    xor eax, eax
    ret