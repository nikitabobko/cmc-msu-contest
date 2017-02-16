%include "io.inc"

section .text
global CMAIN
CMAIN:
	GET_DEC 4, eax
	cdq
	xor eax, edx
	sub eax, edx
	
	PRINT_DEC 4, eax
	NEWLINE

	xor eax, eax
	ret