%include 'io.inc'

section .text
global CMAIN
global reverse
CMAIN:
    mov ebp, esp
    GET_UDEC 4, eax
    GET_UDEC 4, edi
    test edi, edi
    jz .endfor
    .for:
        mov esi, eax
        call reverse
        add eax, esi
        
        dec edi
        test edi, edi
        jnz .for
    .endfor:
    
    mov esi, eax
    call reverse
    cmp esi, eax
    ; if(esi == eax)
    jne .else
        PRINT_STRING  "Yes"
        NEWLINE
        PRINT_UDEC 4, esi
        xor eax, eax
        ret
    .else:
        PRINT_STRING "No"
        xor eax, eax
        ret

; Uses edx, eax, ecx, ebx
; eax = int reverse(unsigned eax)
reverse:
    mov ecx, 10
    xor ebx, ebx
    test eax, eax
    ; while(eax != 0)
    jz .endwhile
    .while:
        xor edx, edx
        div ecx
        
        push eax
        push edx
        xor edx, edx
        mov eax, ebx
        mul ecx
        mov ebx, eax
        pop edx
        pop eax
        
        add ebx, edx
        
        test eax, eax
        jnz .while
    .endwhile:
    mov eax, ebx
    ret