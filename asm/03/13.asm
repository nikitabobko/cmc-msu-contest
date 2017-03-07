%include 'io.inc'

section .text
global CMAIN
CMAIN:
    mov ebp, esp
    GET_UDEC 4, ecx
    mov ebx, ecx
    ; [ebp+4] : temp
    push 0x0
    ; [ebp+8] : mask
    push 0xFFFFFFFF
    
    .loop:
        sub esp, 4
        GET_UDEC 4, [esp]
    loop .loop
    GET_UDEC 4, esi
    mov ecx, 32
    sub ecx, esi
    shr dword [ebp+8], cl
    mov ecx, esi
    
    xor edi, edi
    jnb .endloop1
    .loop1:
        mov eax, [ebp+12+4*edi]
        and eax, [ebp+8]
        
        shr dword [ebp+12+4*edi], cl
        mov ecx, [ebp+4]
        or  dword [ebp+12+4*edi], ecx
        
        mov ecx, 32
        sub ecx, esi
        mov [ebp+4], eax
        shl dword [ebp+4], cl
        mov ecx, esi
        
        cmp edi, ebx
    jb .loop1
    .endloop1:
    
    test ebx, ebx
    je .end
        mov eax, [ebp+4]
        or dword [ebp+12], eax
    .end:
    xor eax, eax
    ret
    
