/*
%include "io.inc"

SECTION .text

GLOBAL CMAIN
CMAIN:
	GET_UDEC        4, EAX

	MOV             EBX, EAX
	DEC             EBX
	XOR             EAX, EBX
	ADD             EAX, 1
	RCR             EAX, 1

	PRINT_UDEC      4, EAX
	NEWLINE

	XOR             EAX, EAX
	RET
*/
#include <stdio.h>

int main(void) {
	unsigned eax;
	scanf("%u", &eax);
	eax ^= (eax-1);
	unsigned mask = 0x0;
	if (eax == 0xFFFFFFFF) {
		mask = 0x80000000;
	}
	printf("%u\n", ((eax+1) >> 1) | mask);
	return 0;
}