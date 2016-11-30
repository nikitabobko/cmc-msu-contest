#include <stdio.h>

int main(void) {
	FILE* in = fopen("input.txt", "r");
	FILE* out = fopen("output.txt", "w");

	int n, m, lastNumber;
	fscanf(in, "%d", &n);
	fscanf(in, "%d", &m);
	for (int i = 0; i < m; i++) {
		fscanf(in, "%d", &lastNumber);
	}
	for (int i = lastNumber; i <= n; i++) {
		fprintf(out, "%d ", i);
	}
	for (int i = 1; i < lastNumber; i++) {
		fprintf(out, "%d ", i);
	}

	fclose(out);
	fclose(in);
	return 0;
}
