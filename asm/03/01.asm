%include "io.inc"

section .text
global CMAIN
CMAIN:
    mov ebp, esp
    
    GET_DEC 4, ecx
    sub ecx, 0x2
    
    GET_DEC 4, ebx
    
    GET_DEC 4, eax
    cmp eax, ebx
    jl .1 
        mov esi, ebx
        mov ebx, eax
        jmp .6
    .1:
        mov esi, eax
    .6:
    
    mov edi, 0x80000000
    
    .loop:
        GET_DEC 4, eax
        cmp eax, ebx
        jl .3
            mov edi, esi
            mov esi, ebx
            mov ebx, eax
            jmp .5
        .3:
        cmp eax, esi
        jl .4
            mov edi, esi
            mov esi, eax
            jmp .5
        .4:
        cmp eax, edi
        jl .5
            mov edi, eax
        .5:
    loop .loop
    
    PRINT_DEC 4, ebx
    PRINT_CHAR ' '
    PRINT_DEC 4, esi
    PRINT_CHAR ' '
    PRINT_DEC 4, edi
    
    xor eax, eax
    ret