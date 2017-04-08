%include 'io.inc'

section .rodata
    mode_scan  db "%u", 0
    ; 10 == '\n'
    mode_print db "0x%08X", 10, 0

section .text
global CMAIN
CEXTERN printf
CEXTERN scanf
CMAIN:
    mov ebp, esp
    sub esp, 12
    lea ebx, [ebp-4]
    
    mov dword   [esp], mode_scan
    mov dword [esp+4], ebx
    call scanf
    ; EOF = -1
    cmp eax, -1
    je .endwhile
    .while:
        mov eax, [ebx]
        mov dword   [esp], mode_print
        mov dword [esp+4], eax
        call printf
        
        mov dword   [esp], mode_scan
        mov dword [esp+4], ebx
        call scanf
        
        ; EOF = -1
        cmp eax, -1
        jne .while
    .endwhile:
    mov esp, ebp
    xor eax, eax
    ret