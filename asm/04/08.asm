%include 'io.inc'

section .text
global CMAIN
global fun
CMAIN:
    mov ebp, esp; for correct debugging
    GET_UDEC 4, eax
    GET_UDEC 4, edi
    mov ebx, eax
    .do_while:
        mov esi, eax
        call fun
        add ebx, eax
        cmp eax, esi
    jne .do_while
    PRINT_UDEC 4, ebx
    xor eax, eax
    ret
    
    
; unsigned eax = fun(unsigned eax, unsigned edi)
fun:
    xor ecx, ecx
    xor edx, edx
    test eax, eax
    jz .endwhile
    ; while (eax != 0)
    .while: 
        xor edx, edx
        div edi
        add ecx, edx
        test eax, eax
        jnz .while
    .endwhile:
    mov eax, ecx
    ret