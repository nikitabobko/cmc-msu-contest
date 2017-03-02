%include "io.inc"

section .bss
	a resd 1
	b resd 1
	c resd 1
	d resd 1
	e resd 1
	f resd 1

section .text
global CMAIN
CMAIN:
	GET_DEC 4, a
	GET_DEC 4, b
	GET_DEC 4, c
	GET_DEC 4, d
	GET_DEC 4, e
	GET_DEC 4, f
	
	xor eax, eax
	
	mov ebx, [e]
	add ebx, [f] 
	imul ebx, [a]
	add eax, ebx
	
	mov ebx, [d]
	add ebx, [f] 
	imul ebx, [b]
	add eax, ebx
	
	mov ebx, [e]
	add ebx, [d] 
	imul ebx, [c]
	add eax, ebx
	
	PRINT_DEC 4, eax
	NEWLINE
	
	xor eax, eax
	ret
