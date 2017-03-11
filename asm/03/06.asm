%include 'io.inc'

section .text
global CMAIN
CMAIN:
    mov ebp, esp
    GET_UDEC 4, eax
    GET_UDEC 4, ecx
    imul ecx, -1
    add ecx, 32
    mov edx, 0xFFFFFFFF    
    shr edx, cl
    mov ebx, ecx
    xor ecx, ecx
    xor edi, edi
    .for:
        mov esi, eax
        and esi, edx
        shr esi, cl
        cmp esi, edi
        cmova edi, esi
        
        shl edx, 1
        inc ecx
        cmp ecx, ebx
        jbe .for
    .endfor:
    PRINT_UDEC 4, edi
    xor eax, eax
    ret