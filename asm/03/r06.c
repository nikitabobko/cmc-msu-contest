/*
%include "io.inc"

section .text
global CMAIN
CMAIN:
	mov EBX, 0xffffffff
again:
	GET_UDEC 4, EAX
	cmp EAX, 0
	jz exit
	cmp EAX, EBX
	ja again
	mov EBX, EAX
	jmp again
exit:   
	PRINT_UDEC 4, EBX
	xor eax, eax
	ret
*/
#include <stdio.h>

int main(void) {
	unsigned ebx = 0xffffffffu;
	while(1) {
		unsigned eax;
		scanf("%u", &eax);
		if (eax == 0) break;
		if (eax > ebx) continue;
		ebx = eax;
	}
	printf("%u\n", ebx);
	return 0;
}