#include <stdio.h>
#include <math.h>

int main(void) {
	unsigned int n, k, max = 0;
	scanf("%u%u", &n, &k);
	unsigned int mask = ((unsigned int)pow(2, k)-1) << (32 - k);
	int i;
	for (i = 0; i < 33 - k; i++) {
		unsigned int val = (n & mask) >> (32 - k - i);
		if (val > max) max = val;
		mask = mask >> 1;
	}
	printf("%u", max);
	return 0;
}
