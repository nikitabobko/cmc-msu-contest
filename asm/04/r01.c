/*
%include "io.inc"

SECTION .text

GLOBAL CMAIN
CMAIN:
	GET_UDEC            4, EAX
	CALL                F
	PRINT_UDEC          4, EAX
	NEWLINE
	XOR                 EAX, EAX
	RET

F:
	CMP                 EAX, 0
	JNZ                 REC
	MOV                 EAX, 1
	RET
REC:
	DEC                 EAX
	CALL                F
	LEA                 EAX, [EAX + 2 * EAX]
	RET
*/
#include <stdio.h>

unsigned f(unsigned eax) {
	return eax == 0 ? 1 : 3*f(--eax);
}

int main(int argc, char const **argv) {
	unsigned eax;
	scanf("%u", &eax);
	printf("%u\n", eax = f(eax));
	return 0;
}