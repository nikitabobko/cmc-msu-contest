%include "io.inc"

section .data
    cards db "23456789TJQKA"
    card_color db "SCDH"

section .text
global CMAIN
CMAIN:
    mov ebp, esp; for correct debugging
    GET_DEC 4, eax
    
    ; eax--
    sub eax, 1
    
    ; ecx = eax
    mov ecx, eax
    
    ; eax /= 13; edx = eax mod 13
    xor edx, edx
    mov ebx, 13
    div ebx
    
    add edx, cards
    PRINT_CHAR [edx]

    add eax, card_color
    PRINT_CHAR [eax]
    
    xor eax, eax
    ret