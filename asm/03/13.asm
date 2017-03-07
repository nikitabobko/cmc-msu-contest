%include 'io.inc'

section .text
global CMAIN
CMAIN:
    mov ebp, esp
    GET_UDEC 4, ecx
    mov ebx, ecx
    ; [ebp-4] : temp
    push 0x0
    ; [ebp-8] : mask
    push 0xFFFFFFFF
    
    jecxz .endloop
    .loop:
        GET_UDEC 4, eax
        push eax
        loop .loop
    .endloop:
    GET_UDEC 4, esi
    mov ecx, 32
    sub ecx, esi
    shr dword [ebp-8], cl
    cmp ecx, 32
    jne .endif ; if(ecx == 32)
        mov dword [ebp-8], 0x0
    .endif:
    mov ecx, esi
    
    xor edi, edi
    cmp edi, ebx
    jnb .endloop1
    .loop1:
        imul edi, -1
        mov eax, [ebp-12+4*edi]
        and eax, [ebp-8]
        
        shr dword [ebp-12+4*edi], cl
        mov ecx, [ebp-4]
        or  dword [ebp-12+4*edi], ecx
        imul edi, -1
        
        mov ecx, 32
        sub ecx, esi
        mov [ebp-4], eax
        shl dword [ebp-4], cl
        mov ecx, esi
        
        inc edi
        cmp edi, ebx
        jb .loop1
    .endloop1:
    
    test ebx, ebx
    je .else
        mov eax, [ebp-4]
        or dword [ebp-12], eax
        
        xor ecx, ecx
        cmp ecx, ebx
        jnb .endloop2
        .loop2:
            imul ecx, -1
            PRINT_UDEC 4, [ebp-12+4*ecx]
            PRINT_CHAR ' '
            imul ecx, -1
            inc ecx
            cmp ecx, ebx
            jb .loop2
        .endloop2:
    .else:
    xor eax, eax
    mov esp, ebp
    ret
    
