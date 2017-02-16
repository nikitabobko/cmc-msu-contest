%include "io.inc"

section .bss
	x resd 1
	n resd 1
	m resd 1
	y resd 1

section .text
global CMAIN

CMAIN:
	GET_DEC 4, x
	GET_DEC 4, n
	GET_DEC 4, m
	GET_DEC 4, y

	sub dword[y], 2011
	mov eax, [n]
	sub eax, dword[m]
	mul dword[y]
	add eax, [x]

	PRINT_DEC 4, eax
	NEWLINE

	xor eax, eax
	ret