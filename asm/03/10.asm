%include 'io.inc'

section .text
global CMAIN
CMAIN:
    mov ebp, esp
    GET_UDEC 4, esi
    mov ecx, 2
    mov ebx, ecx
    imul ebx, ebx
    cmp ebx, esi
    jnbe .endloop
    .loop:
        xor edx, edx
        mov eax, esi
        div ecx
        test edx, edx
        je .print
        
        inc ecx
        mov ebx, ecx
        imul ebx, ebx
        cmp ebx, esi
    jbe .loop
    .endloop:
    PRINT_DEC 4, 1
    xor eax, eax
    ret
    .print:
    PRINT_DEC 4, eax
    xor eax, eax
    ret