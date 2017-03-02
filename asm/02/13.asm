%include "io.inc"

section .bss
	a_x resd 1
	a_y resd 1
	b_x resd 1
	b_y resd 1

section .text
global CMAIN
CMAIN:
	GET_CHAR eax
	GET_DEC 4, ebx
	GET_CHAR ecx
	GET_CHAR ecx
	GET_DEC 4, edx

	sub eax, ecx

	sub ebx, edx

	; Calculate absolute value in eax
	mov ecx, eax
	sar ecx, 31
	xor eax, ecx
	sub eax, ecx

	; Calculate absolute value in ebx
	mov edx, ebx
	sar edx, 31
	xor ebx, edx
	sub ebx, edx

	add eax, ebx

	PRINT_DEC 4, eax
	NEWLINE
	xor eax, eax
	ret	