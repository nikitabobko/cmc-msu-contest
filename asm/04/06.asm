%include 'io.inc'

section .bss
    arr resd 1000

section .text
global CMAIN
global numberOFSignificantZeros
CMAIN:
    mov ebp, esp
    GET_UDEC 4, edx
    xor ecx, ecx
    cmp ecx, edx
    jnb .endfor
    ; for (ecx = 0; ecx < edx; ecx++)   
    .for:
        GET_UDEC 4, [arr+4*ecx]
        inc ecx
        cmp ecx, edx
        jb .for
    .endfor:
    GET_UDEC 4, esi
    xor ecx, ecx
    xor edi, edi
    cmp ecx, edx
    jnb .endfor1
    ; for (ecx = 0; ecx < edx; ecx++)
    .for1:
        mov eax, [arr+4*ecx]
        
        ; backup ecx, edx
        push ecx
        push edx
        
        push eax
        call numberOFSignificantZeros
        add esp, 4
        
        ;restore ecx, edx
        pop edx
        pop ecx
        
        cmp eax, esi
        ; if (eax == esi)
        jne .endif
            inc edi
        .endif:
        
        inc ecx
        cmp ecx, edx
        jb .for1
    .endfor1:
    PRINT_UDEC 4, edi
    xor eax, eax
    ret
    
    
; unsigned numberOfSignificantZeros(unsigned int)
numberOFSignificantZeros:
    push ebp
    mov ebp, esp
    xor ecx, ecx
    mov eax, [ebp+8]
    test eax, eax
    ; while (eax != 0)
    jz .endwhile
    .while:
        shr eax, 1
        ; if (CF == 0)
        jc .endif
            inc ecx
        .endif:
        test eax, eax
        jnz .while
    .endwhile:
    mov eax, ecx
    pop ebp
    ret