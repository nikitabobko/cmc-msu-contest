%include 'io.inc'

section .rodata
    const  dd 0x99999999
    const1 dd 0x19999999

section .bss
    r_ecx resd 1
    r_edx resd 1
    r_eax resd 1
    q_ecx resd 1
    q_edx resd 1
    q_eax resD 1
    ecx0 resd 1
    eax0 resd 1
    edx0 resd 1
    number resb 100

section .text
global CMAIN
CMAIN:
    mov ebp, esp; for correct debugging
    GET_UDEC 4, eax
    GET_UDEC 4, ebx
    GET_UDEC 4, ecx
    mul ecx
    
    ; backup eax
    mov esi, eax
    
    mov eax, edx
    mul ebx
    mov edi, eax
    mov ecx, edx
    
    ; restore eax
    mov eax, esi
    
    mul ebx
    add edx, edi
    ; if (CF == 1)
    jnc .endif
        inc ecx
    .endif:
    mov [ecx0], ecx
    mov [edx0], edx
    mov [eax0], eax
    ; result of multiplication is located in ecx0:edx0:eax0
    
    xor esi, esi
    mov edi, 10
    ; while(ecx:edx:eax != 0)
    test ecx, ecx
    jnz .while
    test eax, eax
    jnz .while
    test edx, edx
    jz .endwhile
    .while:
        ; All this while is just division ecx:edx:eax by 10
        
        mov eax, [eax0]
        xor edx, edx
        div edi
        mov [q_eax], eax
        mov [r_eax], edx
        
        mov eax, [ecx0]
        xor edx, edx
        div edi
        mov [q_ecx], eax
        mov [r_ecx], edx
        
        mov eax, [edx0]
        xor edx, edx
        div edi
        mov [q_edx], eax
        mov [r_edx], edx
        
        ; Calculate eax0
        
        mov eax, [r_edx]
        imul eax, 6
        mov ebx, [r_ecx]
        imul ebx, 6
        add eax, ebx
        add eax, [r_eax]
        
        xor edx, edx
        div edi
        mov byte [number+esi], dl
        mov ebx, eax
        
        
        mov eax, [r_edx]
        mul dword[const1]
        
        add ebx, eax
        
        mov eax, [r_ecx]
        mul dword [const]
        
        add eax, ebx
        ; if (CF == 1) 
        jnc .endif1
            inc edx
        .endif1:
        
        add eax, [q_eax]
        ; if (CF == 1)
        jnc .endif2
            inc edx
        .endif2:
        
        mov [eax0], eax
        mov ebx, edx
        
        ; Calculate edx0
        
        mov eax, [r_ecx]
        mul dword [const1]
        add eax, [q_edx]
        ; if (CF == 1)
        jnc .endif3
            inc edx
        .endif3:   
        add eax, ebx
        ; if (CF == 1)
        jnc .endif4
            inc edx
        .endif4:
        
        mov [edx0], eax
        mov ebx, edx
        
        ; Calculate ecx0
        add ebx, [q_ecx]
        mov [ecx0], ebx

        inc esi
        
        mov eax, [eax0]
        mov edx, [edx0]
        mov ecx, [ecx0]
        
        test ecx, ecx
        jnz .while
        test eax, eax
        jnz .while
        test edx, edx
        jnz .while
    .endwhile:
    
    dec esi
    mov eax, number
    add eax, esi
    cmp esi, 0
    ; while (esi >= 0)
    jnge .endwhile1
    .while1:
        PRINT_DEC 1, [eax] 
        
        dec esi
        dec eax
        cmp esi, 0
        jge .while1
    .endwhile1:
    xor eax, eax
        ret