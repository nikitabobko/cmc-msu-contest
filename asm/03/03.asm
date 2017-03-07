%include 'io.inc'

section .bss
    min resd 500000
    max resd 500000

section .text
global CMAIN
CMAIN:
    mov ebp, esp
    xor ebx, ebx ; ebx - min index
    xor edx, edx ; edx - max index
    GET_UDEC 4, ecx
    cmp ecx, 3
    jnb .else ; if (ecx < 3)
        PRINT_STRING '0 0'
        xor eax, eax
        ret
    .else:
    GET_DEC 4, eax
    GET_DEC 4, esi
    GET_DEC 4, edi
    cmp eax, esi
    jnl .else1; if (esi > eax)
        cmp esi, edi
        jng .else2 ; if (esi > edi)
            mov dword [max+edx], 1
            inc edx
        .else2:
        jmp .endif
    .else1:
    je .endif ; if (esi < edi)
        cmp esi, edi
        jnl .else3 ; if (esi < edi)
            mov dword [min+ebx], 1
            inc ebx
        .else3:
    .endif:
    sub ecx, 3
    push ecx
    mov ecx, 1
    cmp ecx, [ebp-4]
    jnle .endfor
    .for: ; for(int ecx = 1; ecx <= [ebp-4] ; ecx++)
        mov eax, esi
        mov esi, edi
        GET_DEC 4, edi
        cmp esi, eax
        jnl .else4 ; if (esi < eax)
            cmp esi, edi
            jnl .else5 ; if (esi < edi)
                inc ecx
                mov dword [min+4*ebx], ecx
                dec ecx
                inc ebx
            .else5:
            jmp .endif1
        .else4:
        je .endif1 ; if (esi > eax)
            cmp esi, edi
            jng .else6 ; if (esi > edi)
                inc ecx
                mov dword [max+4*edx], ecx
                dec ecx
                inc edx
            .else6:
        .endif1:
        inc ecx
        cmp ecx, [ebp-4]
        jle .for
    .endfor:
    
    PRINT_DEC 4, ebx
    PRINT_CHAR ' '
    xor ecx, ecx
    cmp ecx, ebx
    jnl .endfor1
    .for1:
        PRINT_DEC 4, [min+4*ecx]
        PRINT_CHAR ' '
        inc ecx
        cmp ecx, ebx
        jl .for1
    .endfor1:

    PRINT_DEC 4, edx
    PRINT_CHAR ' '
    xor ecx, ecx
    cmp ecx, edx
    jnl .endfor2
    .for2:
        PRINT_DEC 4, [max+4*ecx]
        PRINT_CHAR ' '
        inc ecx
        cmp ecx, edx
        jl .for2
    .endfor2:
            
    mov esp, ebp
    xor eax, eax
    ret