#include <stdio.h>

int main(void) {
	FILE *in = fopen("matrix.in", "r");
	FILE *out = fopen("trace.out", "w");

	unsigned short n = 0;
	for (int i = 0; i < 2; i++) {
		unsigned char s;
		fread(&s, sizeof(char), 1, in);
		n = (n << 8) | s;
	}
	unsigned long long summ = 0;
	for (int i = 0; i < n; i++) {
		unsigned int num = 0;
		for (int i = 0; i < 4; i++) {
			unsigned char s;
			fread(&s, sizeof(char), 1, in);
			num = (num << 8) | s;
		}
		// 2^31 = Ox80000000
		if (num >= Ox80000000) 0xFFFFFFFFFFFFFFFF
		summ += num;
		for (int i = 0; i < n; i++) {
			fread(&num, sizeof(char), 4, in);
		}
	}
	for (int i = 0; i < 8; i++) {
		unsigned long long s = summ >> ((7 - i) * 8);
		fwrite(&s, sizeof(char), 1, out);
	}

	fclose(out);
	fclose(in);	
	return 0;
}
