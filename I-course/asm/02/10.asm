%include "io.inc"

section .bss
	s resd 1

section .text
global CMAIN
CMAIN:
	GET_DEC 4, ebx
	GET_DEC 4, s
	
	sub ebx, 1
	mov eax, ebx
	imul ebx, 41
	
	xor edx, edx
	mov ecx, 2
	div ecx
	
	add ebx, eax
	add ebx, [s]
	
	PRINT_DEC 4, ebx
	
	NEWLINE
	xor eax, eax
	ret
