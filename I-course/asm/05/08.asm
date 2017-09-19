%include 'io.inc'

section .bss
    array resd 1000

section .rodata
    input      db "input.txt", 0
    read_mode  db "r", 0
    write_mode db "w", 0
    in_format  db "%d", 0
    out_format db "%d ", 0
    output     db "output.txt", 0

section .text
global CMAIN
extern fscanf
extern fopen
extern fclose
extern qsort
extern fprintf
CMAIN:
    mov ebp, esp
    sub esp, 28
    
    ; ebx = fopen("input.txt", "r");
    mov dword   [esp], input
    mov dword [esp+4], read_mode
    call fopen
    mov ebx, eax
    
    mov esi, array
    ; fscanf(ebx, "%d", esi);
    mov dword   [esp], ebx
    mov dword [esp+4], in_format
    mov dword [esp+8], esi
    call fscanf
    cmp eax, -1
    je .endwhile
    ; while(fscanf(ebx, "%d", esi) != EOF)
    .while:
        add esi, 4
        ; fscanf(ebx, "%d", esi);
        mov dword [esp+8], esi
        call fscanf
        cmp eax, -1
        jne .while
    .endwhile:
    
    sub esi, array
    shr esi, 2
    ; qsort(array, esi, sizeof(int)=4, comparator);
    mov dword    [esp], array
    mov dword  [esp+4], esi
    mov dword  [esp+8], 4
    mov dword [esp+12], comparator
    call qsort
    
    ; [ebp-4] = fopen("output.txt", "w");
    mov dword   [esp], output
    mov dword [esp+4], write_mode
    call fopen
    mov [ebp-4], eax
    
    shl esi, 2
    add esi, array
    mov edi, array
    cmp edi, esi
    jnb .endfor
    .for:
        ; fprintf([ebp-4], "%d ", edi);
        mov eax, [ebp-4]
        mov ecx, [edi]
        mov dword   [esp], eax
        mov dword [esp+4], out_format
        mov dword [esp+8], ecx
        call fprintf
        
        add edi, 4
        cmp edi, esi
        jb .for
    .endfor:
    
    ; fclose([ebp-4])
    mov eax, [ebp-4]
    mov [esp], eax
    call fclose
    
    ; fclose(ebx)
    mov [esp], ebx
    call fclose
    
    mov esp, ebp
    xor eax, eax
    ret
    
; int comparator(int a, int b)
comparator:
    mov eax, [esp+4]
    mov ecx, [esp+8]
    mov eax, [eax]
    mov ecx, [ecx]
    cmp eax, ecx
    ; if (a == b)
    jne .endif
        xor eax, eax
        ret
    .endif:
    ; if (a > b)
    jng .else 
        mov eax, 1
        ret
    .else:
        mov eax, -1
        ret