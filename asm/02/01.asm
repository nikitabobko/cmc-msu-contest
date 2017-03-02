%include "io.inc"

section .bss
	vx resd 1
	vy resd 1
	a_x resd 1
	a_y resd 1 
	t resd 1

section .text
global CMAIN

CMAIN:
	GET_DEC 4, vx
	GET_DEC 4, vy
	GET_DEC 4, a_x
	GET_DEC 4, a_y
	GET_DEC 4, t
	
	mov eax, [t]
	imul eax, [t]
	imul eax, [a_x]
	
	mov ebx, [vx]
	imul ebx, [t]
	
	add eax, ebx
	
	PRINT_DEC 4, eax
	
	PRINT_CHAR ' '
	
	mov eax, [t]
	imul eax, [t]
	imul eax, [a_y]
	
	mov ebx, [vy]
	imul ebx, [t]
	
	add eax, ebx
	
	PRINT_DEC 4, eax
	
	NEWLINE
	xor eax, eax
	ret
