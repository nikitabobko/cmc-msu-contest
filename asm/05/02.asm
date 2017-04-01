%include 'io.inc'

section .bss
    str1 resb 1001
    str2 resb 1001
    
section .rodata
    mode_scan db "%s %s", 0
    answer_2 db "2 1", 10, 0
    answer_1 db "1 2", 10, 0
    answer_0 db "0", 10, 0

section .text
extern strstr
extern scanf
extern printf
global CMAIN
CMAIN:
    mov ebp, esp
    and esp, 0xfffffff0
    sub esp, 16
    
    ; scanf("%s %s", str1, str2);
    mov dword   [esp], mode_scan
    mov dword [esp+4], str1
    mov dword [esp+8], str2
    call scanf
    
    ; strstr(str1, str2);
    mov dword   [esp], str1
    mov dword [esp+4], str2
    call strstr
    
    test eax, eax
    ; if (eax != 0)
    je .else
        ; printf("2 1");
        mov dword [esp], answer_2
        call printf
        jmp .endif
    .else:
        ; strstr(str2, str1);
        mov dword   [esp], str2
        mov dword [esp+4], str1
        call strstr
        
        test eax, eax
        ; if (eax != 0)
        je .else1
            ; printf("1 2");
            mov dword [esp], answer_1
            call printf
            jmp .endif1
        .else1:
            ; printf("0");
            mov dword [esp], answer_0
            call printf
        .endif1:
    .endif:
    
    mov esp, ebp
    xor eax, eax
    ret