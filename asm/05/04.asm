%include 'io.inc'

section .rodata
    mode_scan  db "%d", 0
    mode_print db "%u", 10, 0
    file_path  db "data.in", 0
    read_mode  db "r", 0

section .text
global CMAIN
extern fscanf
extern printf
extern fopen
extern fclose
CMAIN:
    mov ebp, esp
    and esp, 0xfffffff0
    sub esp, 16
    
    lea esi, [ebp-4]
    
    ; ebx = fopen("data.in", "r");
    mov dword   [esp], file_path
    mov dword [esp+4], read_mode
    call fopen
    mov ebx, eax
    
    ; fscanf(ebx, "%d", ebp-4);
    mov dword   [esp], ebx
    mov dword [esp+4], mode_scan
    mov dword [esp+8], esi
    call fscanf
    
    xor edi, edi
    cmp eax, -1
    je .endwhile
    ; while (eax != -1)
    .while:
        inc edi    
        
        ; fscanf(ebx, "%d", ebp-4);
        call fscanf
    
        cmp eax, -1
        jne .while
    .endwhile:
    
    ; printf("%u", edi);
    mov dword   [esp], mode_print
    mov dword [esp+4], edi
    call printf
    
    ;fclose(ebx)
    mov dword [esp], ebx
    call fclose
    
    mov esp, ebp
    xor eax, eax
    ret