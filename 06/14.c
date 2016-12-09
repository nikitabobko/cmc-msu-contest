#include <stdio.h>

int main(void) {
	FILE *in = fopen("matrix.in", "r");
	FILE *out = fopen("trace.out", "w");

	int n = 0;
	for (int i = 0; i < 2; i++) {
		int s = 0;
		fread(&s, sizeof(char), 1, in);
		n = (n << 8) | s;
	}
	long long trace = 0;
	for (int i = 0; i < n; i++) {
		int num = 0;
		for (int i = 0; i < 4; i++) {
			int s = 0;
			fread(&s, sizeof(char), 1, in);
			num = (num << 8) | s;
		}
		trace += num;
		// Skip for n numbers
		for (int i = 0; i < n; i++) {
			fread(&num, sizeof(char), 4, in);
		}
	}
	long long mask = 0xFF;
	mask <<= (7 * 8);
	for (int i = 0; i < 8; i++) {
		long long s = (trace & mask) >> ((7 - i) * 8);
		fwrite(&s, sizeof(char), 1, out);
		mask >>= 8;
	}

	fclose(out);
	fclose(in);	
	return 0;
}
