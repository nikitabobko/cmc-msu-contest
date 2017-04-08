%include 'io.inc'

section .rodata
    intFormat db "%d", 0
    outFormat db "%d", 10, 0

section .text
global CMAIN
extern scanf
extern malloc
extern fprintf
extern get_stdout
CMAIN:
    mov ebp, esp
    sub esp, 28
    
    ; scanf("%d", ebp-4)
    lea eax, [ebp-4]
    mov dword   [esp], intFormat
    mov dword [esp+4], eax
    call scanf
    
    mov eax, [ebp-4]
    
    ; ebx = malloc(4*eax)
    shl eax, 2
    mov [esp], eax
    call malloc
    mov ebx, eax
    
    ; for(esi = 0; esi < [ebp-4]; esi++)
    xor esi, esi
    cmp esi, [ebp-4]
    jnb .endfor
    .for:
        ; eax = ebx + 4*esi 
        mov eax, esi
        shl eax, 2
        add eax, ebx 
        
        ; scanf("%d", eax)
        mov dword   [esp], intFormat
        mov dword [esp+4], eax
        call scanf
        
        inc esi
        cmp esi, [ebp-4]
        jb .for
    .endfor:
    
    call get_stdout
    
    ; apply(ebx, [ebp-4], fprintf, 2, stdout, "%d\n")
    mov ecx, [ebp-4]
    mov [esp], ebx
    mov [esp+4], ecx
    mov dword [esp+8], fprintf
    mov dword [esp+12], 2
    mov [esp+16], eax
    mov dword [esp+20], outFormat
    call apply
    
    mov esp, ebp
    xor eax, eax
    ret
    
; apply(int* array, size_t length, void (*fn) (...), int n, ...)
apply:
    push ebp
    mov ebp, esp
    sub esp, 8
    
    ; eax = n
    mov eax, [ebp+20]
    inc eax
    mov edx, eax
    ; alignment
    and edx, 0x3
    sub edx, 4
    sub eax, edx
    shl eax, 2
    sub esp, eax
    mov eax, esp
    
    ; edx points to the parameter after n
    lea edx, [ebp+24]
    
    ; for(ecx = 0; ecx < n; ecx++)
    xor ecx, ecx
    cmp ecx, [ebp+16]
    jnb .endfor1
    .for1:
        mov esi, [edx]
        mov [eax], esi
        
        add eax, 4
        add edx, 4
        
        inc ecx
        cmp ecx, [ebp+20]
        jb .for1
    .endfor1:
    
    ; now edi = eax points to where we should put (n+1)'th parameter of function
    mov edi, eax
    
    ; for(esi = 0; esi < lenght; i++)
    xor esi, esi
    cmp esi, [ebp+12]
    jnb .endfor
    .for:
        ; eax = array[esi]
        mov eax, esi
        shl eax, 2
        add eax, [ebp+8]
        mov eax, [eax] 
        
        mov [edi], eax
        mov eax, [ebp+16]
        call eax
        
        inc esi
        cmp esi, [ebp+12]
        jb .for
    .endfor:   
    
    leave
    ret