#include <stdio.h>

int main(void) {
	unsigned int n;
	scanf("%u", &n);
	printf("%u", ((~(n >> 24)) << 24) | (n & 0xFFFFFF));
	return 0;
}
