%include 'io.inc'

section .rodata
    fileName  db "input.bin", 0
    readMode  db "rb", 0
    intFormat db "%d ", 0

section .bss
    tree          resd 30000
    removedNodes  resd 30000

section .text
global CMAIN
extern fopen
extern fread
extern fclose
CMAIN:
    mov ebp, esp
    sub esp, 28
    
    ; ebx = fopen("input.bin", "rb")
    mov dword [esp], fileName
    mov dword [esp+4], readMode
    call fopen
    mov ebx, eax
    
    xor esi, esi
    ; while(1)
    .while:
        ; fread(tree+esi, 4, 3, ebx)
        lea eax, [tree + esi]
        mov [esp], eax
        mov dword [esp+4], 4
        mov dword [esp+8], 3
        mov [esp+12], ebx
        call fread
        
        cmp eax, 0
        ; if (eax == 0) break;
        je .endwhile
        
        add esi, 12
        
        jmp .while
    .endwhile:
    
    xor edi, edi
    cmp edi, esi
    jnb .endfor
    ; for(edi = 0; edi < esi; edi+=12)
    .for:
        mov [esp], edi
        call removeChildren
        
        add edi, 12
        cmp edi, esi
        jb .for
    .endfor: 
    
    xor edi, edi
    cmp edi, esi
    jnb .endfor1
    ; for(edi = 0; edi < esi; edi += 12)
    .for1:
        mov eax, [removedNodes+edi]
        test eax, eax
        ; if (eax == 0) break;
        jz .endfor1
        
        add edi, 12
        cmp edi, esi
        jb .for1
    .endfor1:
    
    mov [esp], edi
    call printTree
    
    ; fclose(ebx)
    mov [esp], ebx
    call fclose
    
    mov esp, ebp
    ret
    
printTree:
    push ebp
    mov ebp, esp
    sub esp, 24
    
    ; backup ebx
    mov [ebp-4], ebx
    
    mov ebx, [ebp+8]
    mov eax, [tree+ebx]
    ; printf("%d ", eax)
    mov dword [esp], intFormat
    mov [esp+4], eax
    call printf
    
    mov eax, [tree+ebx+4]
    cmp eax, -1
    ; if (eax != -1)
    je .endif
        mov [esp], eax
        call printTree
    .endif:
    
    mov eax, [tree+ebx+8]
    cmp eax, -1
    ; if (eax != -1)
    je .endif1
        mov [esp], eax
        call printTree
    .endif1:
    
    ; restore ebx
    mov ebx, [ebp-4]
    
    leave
    ret
    
    
; this function gets node's offset as parameter
removeChildren:
    push ebp
    mov ebp, esp
    
    ; backup ebx, esi, edi
    push ebx
    push esi
    push edi
    
    mov ebx, [ebp+8]
    mov esi, [removedNodes + ebx]
    test esi, esi
    ; if (esi != 0)
    jz .endif
        ; restore ebx, esi, edi
        pop edi
        pop esi
        pop ebx
        
        leave
        ret
    .endif:
    
    ; remove left children
    add ebx, 4
    mov edi, [tree + ebx]
    cmp edi, -1
    ; if (edi != -1)
    je .endif1
        push edi
        call removeChildren
        add esp, 4
        
        mov dword [removedNodes + edi], 1
    .endif1:
    
    ; remove right children
    add ebx, 4
    mov edi, [tree + ebx]
    cmp edi, -1
    ; if (edi != -1)
    je .endif2
        push edi
        call removeChildren
        add esp, 4
        
        mov dword [removedNodes + edi], 1
    .endif2:
    
    ; restore ebx, esi, edi
    pop edi
    pop esi
    pop ebx
    
    leave
    ret