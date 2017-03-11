/*
%include "io.inc"

SECTION .bss
	N               RESD 1
	K               RESD 1

	a               RESD 21
	b               RESD 21

SECTION .text

GLOBAL CMAIN
CMAIN:
	GET_DEC         4, N
	GET_DEC         4, K

	MOV             ESI, a
	MOV             EDI, b

	MOV             DWORD [ESI], 1
	XOR             ECX, ECX

.1:
	CMP             ECX, DWORD [N]
	JZ              .4

	INC             ECX 

	MOV             DWORD [EDI], 1
	XOR             EDX, EDX

.2:
	CMP             EDX, ECX
	JZ              .3

	INC             EDX

	MOV             EAX, DWORD [ESI + 4 * EDX]
	ADD             EAX, DWORD [ESI + 4 * EDX - 4]

	MOV             DWORD [EDI + 4 * EDX], EAX

	JMP             .2

.3:
	MOV             DWORD [EDI + 4 * EDX], 1

	MOV             EAX, ESI
	MOV             ESI, EDI
	MOV             EDI, EAX

	JMP             .1

.4:
	MOV             EDI, DWORD [K]
	MOV             EAX, DWORD [ESI + 4 * EDI]

	PRINT_DEC       4, EAX
	NEWLINE

	XOR             EAX, EAX
	RET
*/
#include <stdio.h>
#include <stdlib.h>

int main(void) {
	int n, k, ecx = 0;
	scanf("%d %d", &n, &k);
	int *a = malloc(21*sizeof(int)), *b = malloc(21*sizeof(int));
	a[0] = 1;
	while(ecx != n) {
		ecx++;
		b[0] = 1;
		int edx = 0;
		while(edx != ecx) {
			edx++;
			b[edx] = a[edx] + a[edx-1];
		}
		b[edx] = 1;
		int *t = a;
		a = b;
		b = t;
	}
	printf("%d\n", a[k]);
	return 0;
}