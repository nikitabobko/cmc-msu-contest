%include 'io.inc'

section .text
global CMAIN
CMAIN:
    mov ebp, esp; for correct debugging
    GET_UDEC 4, ebx
    xor edi, edi
    mov esi, 1
    .loop:
        push esi
        call isDeficientNumber
        add  esp, 4
        
        test eax, eax
        ; if(isDeficientNumber(esi)) 
        jz .endif
            inc edi
            cmp edi, ebx
            ; if (edi == ebx)
            jne .endif1
                PRINT_UDEC 4, eSi
                xor eax, eax
                ret
            .endif1:
        .endif:
        
        inc esi
    jmp .loop

    
; cdecl
; bool (char) isDeficientNumber(unsigned n)
isDeficientNumber:
    push ebp
    mov ebp, esp
    
    ; backup ebx
    push ebx
    
    xor ebx, ebx
    mov ecx, 1
    cmp ecx, [ebp+8]
    jnb .endfor
    ; for(ecx = 1; ecx < n; ++ecx)
    .for:
        xor edx, edx
        mov eax, [ebp+8]
        div ecx
        test edx, edx
        ; if(edx == 0)
        jne .endif
            add ebx, ecx
        .endif:
        cmp ebx, [ebp+8]
        ; if(ebx >= n)
        jnae .endif1
            ; break
            jmp .endfor
        .endif1:
        
        inc ecx
        cmp ecx, [ebp+8]
        jb .for
    .endfor:
    
    mov eax, 0
    cmp ebx, [ebp+8]
    ; if (ebx < n)
    jnb .endif2
        mov eax, 0x1
    .endif2:
    
    ;restore ebx
    pop ebx
    
    pop ebp
    ret