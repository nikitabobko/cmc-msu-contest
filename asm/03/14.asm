%include 'io.inc'

section .text
global CMAIN
CMAIN:
    mov ebp, esp
    GET_UDEC 4, ecx
    GET_UDEC 4, ebx
    
    xor edi, edi
    
    .loop:
        mov eax, ecx
    
        xor esi, esi
        .1:
            test eax, eax
            jz .end
    
            mov edx, eax
            and edx, 0x1
            xor edx, 0x1
            add esi, edx
            
            shr eax, 1
    
            jmp .1
        .end:
        
        cmp esi, ebx
        jne .2
            inc edi
        .2:
    
    loop .loop
    
    PRINT_DEC 4, edi
    
    xor eax, eax
    ret