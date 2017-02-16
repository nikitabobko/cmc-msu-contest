%include "io.inc"

section .bss
	a resd 1
	b resd 1
	c resd 1
	v resd 1

section .text
global CMAIN

CMAIN:
	GET_DEC 4, a
	GET_DEC 4, b
	GET_DEC 4, c
	GET_DEC 4, v
	
	mov eax, [a]
	mul dword[b]
	
	mov ecx, edx
	imul ecx, [c]
	mul dword[c]
	add edx, ecx
	
	;mov ecx, [v]
	
	div dword[v]
	
	PRINT_DEC 4, eax
	
	NEWLINE
	xor eax, eax
	ret
