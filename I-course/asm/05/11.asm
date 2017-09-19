%include 'io.inc'

; struct list {
;     int val;
;     list* next;
;     list* previous
; }

section .rodata
    inFile    db "input.txt", 0
    outFile   db "output.txt", 0
    intFormat db "%u", 0
    outFormat db "%d ", 0
    readMode  db "r", 10
    writeMode db "w", 10
    
section .bss
    arr resd 1000000

section .text
global CMAIN
extern malloc
extern fopen
extern fscanf
extern fclose
extern free
extern fprintf
CMAIN:
    mov ebp, esp
    sub esp, 28
    
    ; [ebp-8] = fopen("input.txt", "r")
    mov dword [esp], inFile
    mov dword [esp+4], readMode
    call fopen
    mov [ebp-8], eax
    
    ; fscanf([ebp-8], "%d", ebp-4)
    lea eax, [ebp-4]
    mov edx, [ebp-8]
    mov [esp], edx
    mov dword [esp+4], intFormat
    mov [esp+8], eax
    call fscanf
    mov esi, [ebp-4]
    
    ; ebx keeps list's head
    xor ebx, ebx
    
    ; fills list
    test esi, esi
    je .endwhile
    .while:
        mov [esp], ebx
        mov [esp+4], esi
        call addToHead
        mov ebx, eax
        
        dec esi
        mov [arr+4*esi], ebx
        test esi, esi
        jne .while
    .endwhile:
    
    ; fscanf([ebp-8], "%d", ebp-4)
    lea eax, [ebp-4]
    mov edx, [ebp-8]
    mov [esp], edx
    mov dword [esp+4], intFormat
    mov [esp+8], eax
    call fscanf
    mov esi, [ebp-4]
    
    test esi, esi
    je .endwhile1
    .while1:
        ; fscanf([ebp-8], "%d", ebp-4)
        lea eax, [ebp-4]
        mov edx, [ebp-8]
        mov [esp], edx
        mov dword [esp+4], intFormat
        mov [esp+8], eax
        call fscanf
        mov edi, [ebp-4]
        ; fscanf([ebp-8], "%d", ebp-4)
        call fscanf
        
        mov [esp], ebx
        mov [esp+4], edi
        mov edi, [ebp-4]
        mov [esp+8], edi
        call apply
        mov ebx, eax
        
        dec esi
        test esi, esi
        jne .while1
    .endwhile1:
    
    ; esi = fopen("output.txt", "w")
    mov dword [esp], outFile
    mov dword [esp+4], writeMode
    call fopen
    mov esi, eax
    
    .while2:
        mov eax, [ebx]
        ; fprintf(esi, outFormat, eax)
        mov [esp], esi
        mov dword [esp+4], outFormat
        mov [esp+8], eax
        call fprintf
        
        mov [esp], ebx
        mov ebx, [ebx+4]
        call free
        
        test ebx, ebx
        je .endwhile2
        jmp .while2
    .endwhile2:
    
    ; fclose(esi)
    mov [esp], esi
    call fclose
    
    ; fclose(ebp-8)
    mov eax, [ebp-8]
    mov dword [esp], eax
    call fclose
    
    mov esp, ebp
    xor eax, eax
    ret
    
; list* apply(list* head, int firstParameter, int secondParameter)
apply:
    push ebp
    mov ebp, esp
    
    mov eax, [ebp+8]
    test eax, eax
    jne .endif
        leave
        ret
    .endif:
    
    mov edx, [ebp+12]
    mov ecx, [eax]
    cmp ecx, edx
    jne .endif1
        leave
        ret
    .endif1:
    
    dec edx
    mov edx, [arr+4*edx]
    mov edx, [edx+8]
    
    mov eax, [ebp+16]
    dec eax
    mov eax, [arr+4*eax]
    
    mov ecx, [edx+4]
    push ecx
    
    mov ecx, [eax+4]
    mov [edx+4], ecx
    test ecx, ecx
    je .endif2
        mov [ecx+8], edx
    .endif2:
    
    mov ecx, [ebp+8]
    mov [eax+4], ecx
    mov [ecx+8], eax
    
    pop eax
    
    mov dword [eax+8], 0
    
    leave
    ret
    
; adds node to the head of the list
; list* addToHead(list* head, int value) 
addToHead:
    push ebp
    mov ebp, esp
    sub esp, 8
    
    mov dword [esp], 12
    call malloc
    mov edx, [ebp+12]
    mov [eax], edx
    mov edx, [ebp+8]
    mov [eax+4], edx
    test edx, edx
    je .endif
        mov [edx+8], eax
    .endif:
    mov dword [eax+8], 0
    
    leave
    ret
