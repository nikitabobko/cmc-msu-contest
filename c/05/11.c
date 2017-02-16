#include <stdio.h>
#include <stdlib.h>

void deallocateMatrix(unsigned int** matrix, int n) {
	for (int i = 0; i < n; i++) {
		free(matrix[i]);
	}
	free(matrix);
}

unsigned int** allocateMatrix(int n, int m) {
	unsigned int** arr = malloc(n * sizeof(unsigned int*));
	for (int i = 0; i < n; i++) {
		arr[i] = malloc(m * sizeof(unsigned int));
	}
	return arr;
}

int main(void) {
	int n, m;
	scanf("%d%d", &n, &m);
	unsigned int** arr = allocateMatrix(n, m);
	unsigned int** way = allocateMatrix(n, m);
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			scanf("%u", &(arr[i][j]));
		}
	}
	for (int i = 0; i < n; i++) {
		way[i][0] = arr[i][0];
	}
	for (int i = 1; i < m; i++) {
		unsigned int max = 0;
		for (int k = 0; k < n; k++) {
			unsigned int localMax = 0;
			for (int j = -1; j < 2; j++) {
				if (j + k >= 0 && j + k < n && way[j + k][i - 1] > localMax)
					localMax = way[j + k][i - 1];
			}
			way[k][i] = localMax + arr[k][i];
			if (way[k][i] > max) max = way[k][i];
		}
		if (i == m - 1) printf("%u\n", max);
	}
	deallocateMatrix(arr, n);
	deallocateMatrix(way, n);
	return 0;
}
