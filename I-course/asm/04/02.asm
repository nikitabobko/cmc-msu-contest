%include 'io.inc'

section .rodata
    c dd 3

section .text
global CMAIN
global  F
CMAIN:
    mov ebp, esp
    GET_UDEC 4, eax
    push eax
    call F
    add esp, 4
    PRINT_UDEC 4, eax
    xor eax, eax
    ret

; cdecl
F:
    push ebp
    mov ebp, esp
    
    mov eax, [ebp+8]
    test eax, eax
    ; if (eax != 0)
    jz .endif1
        ; backup ebx
        push ebx
    
        xor ebx, ebx
        xor edx, edx
        div dword [c]
        cmp edx, 1
        ; if (edx == 1)
        jne .endif
            inc ebx
        .endif:
    
        push eax
        call F
        add esp, 4
        
        add eax, ebx
        
        ;restore ebx
        pop ebx
    .endif1:
    pop ebp
    ret
    