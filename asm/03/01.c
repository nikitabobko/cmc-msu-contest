/*
%include "io.inc"

SECTION .text

GLOBAL CMAIN
CMAIN:
    GET_DEC         4, ECX

    MOV             EBX, 1
    XOR             EAX, EAX

.L:
    XOR             EAX, EBX
    XOR             EBX, EAX
    XOR             EAX, EBX

    ADD             EBX, EAX
    LOOP            .L

    PRINT_UDEC      4, EAX
    NEWLINE

    XOR             EAX, EAX
    RET
*/
#include <stdio.h>

int main(void) {
	int n;
	unsigned ebx = 1, eax = 0;
	scanf("%d", &n);
	do {
		eax ^= ebx;
		ebx ^= eax;
		eax ^= ebx;
		ebx += eax;
		n--;
	} while (n > 0);
	printf("%u\n", eax);
	return 0;
}