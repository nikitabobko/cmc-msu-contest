%include 'io.inc'

section .bss
    arr resb 11

section .text
global CMAIN
CMAIN:  
    mov ebp, esp
    GET_UDEC 4, ebx
    test ebx, ebx
    jne .endif
        PRINT_CHAR '0'
        xor eax, eax
        ret
    .endif:
    xor ecx, ecx
    ; do { ... } while (ecx > 0)
    .while:
        mov al, 0x7
        and al, bl
        mov byte[arr+ecx], al
        inc ecx
        
        shr ebx, 3 
        test ebx, ebx
        jne .while
    .endwhile:
    .loop:
        mov eax, arr
        dec ecx
        add eax, ecx
        inc ecx
        PRINT_DEC 1, [eax]
    loop .loop
    xor eax, eax
    ret