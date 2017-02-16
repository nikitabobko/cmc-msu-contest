#include <stdio.h>

int main(void) {
	char s[4];
	int i;
	scanf("%s", s);

	// Convert to DEC
	int num = 0;
	for (i = 0; i < 4; i++) {
		int c = s[3-i];
		num = num | (( (c >= '0' && c <= '9') ? c - '0' : c - 'a' + 10 ) << i*4);
	}

	// Transposition
	int numT = 0;
	for (i = 0; i < 4; i++) {
		int mask = 0x8000 >> i;
		int j, numS = 0;
		for (j = 0; j < 4; j++) {
			int n = (num & mask) >> ((3-i) + (3*(3-j)));
			numS = numS | n;
			mask = mask >> 4;
		}
		numT = numT | (numS << (4*(3-i)));
	}

	// Convert back to HEX
	for (i = 0; i < 4; i++) {
		int c = (numT & (0xF << i*4)) >> (4*i);
		s[3-i] = (c >= 0 && c <= 9) ? c + '0' : c - 10 + 'a';
	}

	printf("%s\n", s);
	return 0;
}