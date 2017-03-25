/*
%include "io.inc"

SECTION .data
	A DD F1, F2, 0, F3, 0, F4

SECTION .text

GLOBAL CMAIN
CMAIN:
	PUSH	        EBP
	GET_DEC	        4, EAX
	GET_CHAR	    CL
	GET_DEC	        4, EBX
	SUB     	    CL, 42
	MOVZX   	    EBP, CL
	LEA     	    EBP, [A + 4 * EBP]
	CALL    	    [EBP]
	PRINT_DEC       4, EAX
	NEWLINE
	POP		        EBP
	XOR     	    EAX, EAX
	RET

F1:
	IMUL    	    EAX, EBX
	RET

F2:
	ADD     	    EAX, EBX
	RET

F3:
	SUB     	    EAX, EBX
	RET

F4:
	CDQ
	IDIV    	    EBX
	RET
*/
#include <stdio.h>

int f1(int eax, int ebx) {
	return eax * ebx;
}

int f2(int eax, int ebx) {
	return eax + ebx;
}

int f3(int eax, int ebx) {
	return eax - ebx;
}

int f4(int eax, int ebx) {
	return eax / ebx;
}

int main(int argc, char const **argv) {
	// Array of pointers to functions
	int (*fun[])(int, int) = {f1, f2, NULL, f3, NULL, f4};
	int eax, ebx;
	unsigned char ecx;
	scanf("%d %c %d", &eax, &ecx, &ebx);
	// Ohh, my GOD!
	printf("%d\n", (fun[(int) (ecx -= 42)])(eax, ebx));
	return 0;
}