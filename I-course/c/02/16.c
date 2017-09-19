#include <stdio.h>

int main(void){
	int n;
	scanf("%d", &n);
	int arr[n];
	int i;
	for (i = 0; i < n; i++) scanf("%d", arr + i);
	int mask = 1;
	// Hard algorithm, don't try to understand! Even I've forgotten how it works.
	for (i = 0; 1; i++, mask <<= 1) {
		int xor[] = {0, 0, 0, 0};
		int j;
		for (j = 0; j < n; j++) {
			if ((arr[j] & mask) >> i) xor[0] ^= arr[j];
			else xor[1] ^= arr[j];
		}
		if (xor[0] && xor[1]) {
			for (j = 0; 1; j++, xor[0] >>= 1) if (xor[0] & 1) break;
			int whichBit1 = j;
			for (j = 0; 1; j++, xor[1] >>= 1) if (xor[1] & 1) break;
			int whichBit2 = j;
			xor[0] = 0;
			xor[1] = 0;
			for (j = 0; j < n; j++) 
				if ((arr[j] & mask) >> i) {
					if ((arr[j] & (1 << whichBit1)) >> whichBit1) xor[0] ^= arr[j];
					else xor[1] ^= arr[j];
				} else {
					if ((arr[j] & (1 << whichBit2)) >> whichBit2) xor[2] ^= arr[j];
					else xor[3] ^= arr[j];
				}
			// Just sort xor[0], xor[1], xor[2], xor[3]. One of them is necessarily zero
			for (j = 1; j < 4; j++) 
				if (xor[j-1] > xor[j]) {
					int t = xor[j];
					xor[j] = xor[j-1];
					xor[j-1] = t;
					j = 0;
				}
			printf("%d %d %d\n", xor[1], xor[2], xor[3]);
			return 0;
		}
	}
}
