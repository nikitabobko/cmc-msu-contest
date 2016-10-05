#include <stdio.h>

int main(void) {
	unsigned int n, i, xor = 0, xor1 = 0, xor2 = 0;
	scanf("%d", &n);
	unsigned int arr[n];
	for (i = 0; i < n; i++) {
		scanf("%u", arr + i);
		xor ^= arr[i];
	}
	for (i = 0; xor > 0; i++, xor >>= 1) if (xor & 1) break;
	unsigned int whichBit = i;
	for (i = 0; i < n; i++) {
		if ((arr[i] & (1 << whichBit)) >> whichBit) xor1 ^= arr[i];
		else xor2 ^= arr[i];
	}
	printf("%d %d\n", xor1 < xor2 ? xor1 : xor2, xor1 < xor2 ? xor2 : xor1);
	return 0;
}