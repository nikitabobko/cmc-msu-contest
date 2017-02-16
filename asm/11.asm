%include "io.inc"

section .bss
	width resd 1
	height resd 1
	width2x resd 1
	height2x resd 1
	originalWidth resd 1
	originalHeight resd 1

section .text
global CMAIN
	
CMAIN:
	;GET_CHAR eax
	;GET_DEC 4, ebx
	mov eax, 'H'
	mov ebx, 2

	mov [originalWidth], eax
	mov [originalHeight], ebx

	imul eax, -1
	add eax, 'H' + 'A'

	imul ebx, -1
	add ebx, 8

	mov ecx, 2
	xor edx, edx
	div ecx
	mov [width], eax
	; mov ecx, 2
	mul ecx
	mov [width2x], eax

	mov eax, ebx
	; mov ecx, 2
	xor edx, edx
	div ecx
	mov [height], eax
	; mov ecx, 2
	mul ecx
	mov [height2x], eax

	; Main calculations

	mov eax, [originalWidth]
	sub eax, width2x
	mul dword[height]

	mov ebx, [originalHeight]
	sub ebx, [height2x]
	imul ebx, [width]

	add eax, ebx

	mov ebx, width2x
	imul ebx, height

	add eax, ebx

	PRINT_DEC 4, eax

	NEWLINE
	xor eax, eax
	ret