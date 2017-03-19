%include 'io.inc'

section .text
global CMAIN
global div3
CMAIN:
    mov ebp, esp; for correct debugging
    GET_UDEC 4, ebx
    .while:
        GET_UDEC 4, eax
        call div3
        test eax, eax
        ; if (eax == 0)
        jnz .else
            PRINT_STRING "NO"
            NEWLINE
            jmp .endif
        .else:
            PRINT_STRING "YES"
            NEWLINE
        .endif:
    
        dec ebx
        test ebx, ebx
    jne .while
    xor eax, eax
    ret
    
; uses esi
; bool eax = div3(unsigned eax)
div3:
    xor ecx, ecx
    xor edx, edx
    ; if (eax < 3 && eax != 0)
    test eax, eax
    jz .endif2
    cmp eax, 3
    jnb .endif2
        mov eax, 0x0
        ret
    .endif2:
    test eax, eax
    je .endwhile
    ; while(eax != 0)
    .while:
        mov esi, eax
        and esi, 0x1
        test ecx, ecx
        ; if (ecx == 0)
        jne .else
            add edx, esi 
            jmp .endif
        .else:
            sub edx, esi
        .endif:
        
        not ecx
        shr eax, 1
        test eax, eax
        jne .while
    .endwhile:
    mov eax, edx
    ; Absolute value of eax
    cdq
    xor eax, edx
    sub eax, edx
    
    test eax, eax
    ; if (eax == 0)
    jnz .endif1
        mov eax, 1
        ret
    .endif1:
    call div3
    ret