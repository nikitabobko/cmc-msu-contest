/*
%include "io.inc"

section .data

section .text
global CMAIN
CMAIN:
	GET_UDEC 4, ECX
	MOV EDX, 0
	SHR ECX, 1
	JC END

	L1:
	GET_DEC 4, EAX
	GET_DEC 4, EBX
	IMUL EAX, EBX
	ADD EDX, EAX
	DEC ECX
	JNZ L1

	END:
	PRINT_DEC 4, EDX
	NEWLINE
	XOR EAX, EAX
	RET
*/
#include <stdio.h>

int main(void) {
	unsigned ecx;
	scanf("%u", &ecx);
	int edx = 0;
	if ((ecx & 0x1) == 0) {
		ecx >>= 1;
		do {
			int eax, ebx;
			scanf("%d %d", &eax, &ebx);
			edx += eax * ebx;
			ecx--;
		} while(ecx != 0);
	}
	printf("%d\n", edx);
	return 0;
}