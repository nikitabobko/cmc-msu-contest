#include <stdio.h>

int main(void) {
	FILE *in = fopen("matrix.in", "r");
	FILE *out = fopen("trace.out", "w");

	short n = 0;
	for (int i = 0; i < 2; i++) {
		char s;
		fread(&s, sizeof(char), 1, in);
		n = (n << 8) | s;
	}
	long long summ = 0;
	for (int i = 0; i < n; i++) {
		int num = 0;
		for (int i = 0; i < 4; i++) {
			char s;
			fread(&s, sizeof(char), 1, in);
			num = (num << 8) | s;
		}
		summ += num;
		for (int i = 0; i < n; i++) {
			fread(&num, sizeof(char), 4, in);
		}
	}
	for (int i = 0; i < 8; i++) {
		long long s = summ >> ((7 - i) * 8);
		fwrite(&s, sizeof(char), 1, out);
	}

	fclose(out);
	fclose(in);	
	return 0;
}
