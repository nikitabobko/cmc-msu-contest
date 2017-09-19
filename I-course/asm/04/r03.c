/*
%include "io.inc"

SECTION .text

GLOBAL CMAIN
CMAIN:
	SUB     	    ESP, 12
	GET_DEC 	    4, [ESP]
	GET_DEC 	    4, [ESP + 4]
	GET_DEC 	    4, [ESP + 8]
	CALL    	    M
	ADD     	    ESP, 12
	PRINT_DEC       4, EAX
	NEWLINE
	XOR     	    EAX, EAX
	RET

M:
	MOV     	    EAX, DWORD [ESP + 4]
	MOV     	    ECX, DWORD [ESP + 8]
	MOV     	    EDX, DWORD [ESP + 12]
	.1:
		CMP     	    EAX, ECX
		; if (EAX < ECX)
		JGE     	    .2
			XOR     	    EAX, ECX
			XOR     	    ECX, EAX
			XOR     	    EAX, ECX
		.2:
		CMP     	    EAX, EDX
		; if (EAX <= EDX)
		JG      	    .3
			RET
		.3:
		XOR     	    EAX, EDX
		XOR     	    EDX, EAX
		XOR     	    EAX, EDX
	JMP     	    .1
*/

#include <stdio.h>

int m(int a, int b, int c) {
	if ((a >= b && a <= c) || (a >= c && a <= b))
		return a;
	else if ((b >= a && b <= c) || (b >= c && b <= a))
		return b;
	else if ((c >= a && c <= b) || (c >= b && c <= a))
		return c;
	return 0;
}

int main(int argc, char const *argv[]) {
	int a, b, c;
	scanf("%d %d %d", &a, &b, &c);
	printf("%d\n", m(a, b, c));
	return 0;
}