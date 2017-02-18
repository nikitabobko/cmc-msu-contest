%include "io.inc"

section .text
global CMAIN
CMAIN:
	GET_DEC 4, eax
	mov ebx, eax
	sar ebx, 31
	xor eax, ebx
	sub eax, ebx
	
	PRINT_DEC 4, eax
	NEWLINE

	xor eax, eax
	ret