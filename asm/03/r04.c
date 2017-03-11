/*
%include "io.inc"

SECTION .data
        mask    DD      0xffff, 0xff00ff, 0xf0f0f0f, 0x33333333, 0x55555555

SECTION .text
GLOBAL CMAIN
CMAIN:
    GET_UDEC        4, EAX
    MOV             EBX, mask
    ADD             EBX, 16
    MOV             ECX, 1
.L:
    MOV             ESI, DWORD [EBX]
    MOV             EDI, ESI
    NOT             EDI
    MOV             EDX, EAX
    AND             EAX, ESI
    AND             EDX, EDI
    SHL             EAX, CL
    SHR             EDX, CL
    OR              EAX, EDX
    SHL             ECX, 1
    SUB             EBX, 4
    CMP             EBX, mask - 4
    JNE             .L

    PRINT_UDEC      4, EAX
    NEWLINE
    XOR             EAX, EAX
    RET
*/
#include <stdio.h>

int main(void) {
	unsigned mask[] = {0xffff, 0xff00ff, 0xf0f0f0f, 0x33333333, 0x55555555};
	unsigned eax, ebx = 4, ecx = 1;
	scanf("%u", &eax);
	do {
		eax = ((eax & mask[ebx]) << ecx) | ((eax & ~mask[ebx]) >> ecx);
		ecx <<= 1;
		ebx--;
	} while(ebx != -1);
	printf("%u\n", eax);
	return 0;
}