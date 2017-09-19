%include 'io.inc'

section .rodata
    intFormat db "%d", 0
    strFormat db '%s', 0

section .bss
    arr resd 500

section .text
extern scanf
extern malloc
extern strmp
extern free
extern strcmp
global CMAIN
CMAIN:
    mov ebp, esp
    sub esp, 28
    
    ; [ebp-12] keeps number of different strings
    mov dword [ebp-12], 0
    
    ; scanf("%d", ebp-4)
    lea eax, [ebp-4]
    mov dword   [esp], intFormat
    mov dword [esp+4], eax
    call scanf
    
    ; esi keeps number of elements in arr
    xor esi, esi
    
    mov ebx, [ebp-4]
    ; while (ebx != 0)
    test ebx, ebx
    je .endwhile
    .while:
        ; eax = malloc(11)
        mov dword [esp], 11
        call malloc
        
        ; backup ebx
        mov [ebp-8], ebx
        
        mov ebx, eax
        ; scanf("%s", ebx)
        mov dword   [esp], strFormat
        mov dword [esp+4], ebx
        call scanf
        
        ; for(edi = 0; edi < esi; edi++)
        xor edi, edi
        cmp edi, esi
        jnb .endfor
        .for:
            ; strcmp(arr+4*edi, ebx)
            lea eax, [arr+4*edi]
            mov eax, [eax]
            mov dword   [esp], eax
            mov dword [esp+4], ebx
            call strcmp
            
            test eax, eax
            ; if (eax == 0)
            jnz .endif
                ; free(ebx)
                mov [esp], ebx
                call free
                
                ; interrupt for loop
                jmp .interruptfor
            .endif:
            
            inc edi
            cmp edi, esi
            jb .for
        .endfor:
            ; if for loop wasn't interrupted
            mov eax, [ebp-12]
            inc eax
            mov [ebp-12], eax
            
            ; put new scanned string to the top of arr
            lea eax, [arr+4*esi]
            mov dword [eax], ebx
            inc esi
        .interruptfor:
        
        ; restore ebx
        mov ebx, [ebp-8]
        
        dec ebx
        test ebx, ebx
        jne .while
    .endwhile:
    
    dec esi
    ; while(esi != -1)
    cmp esi, -1
    je .endwhile1
    .while1:
        ; free(arr+esi)
        lea eax, [arr+4*esi]
        mov eax, [eax]
        mov [esp], eax
        call free
        
        dec esi
        cmp esi, -1
        jne .while1
    .endwhile1:
    
    mov eax, [ebp-12]
    ; printf("%d", eax)
    mov dword   [esp], intFormat
    mov dword [esp+4], eax
    call printf
    
    mov esp, ebp
    xor eax, eax
    ret