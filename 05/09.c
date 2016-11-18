#include <stdio.h>
#include <stdlib.h>

int** allocateMatrix(int n, int m) {
	int** matrix = malloc(n * sizeof(int*));
	for (int i = 0; i < n; i++) {
		matrix[i] = calloc(m, sizeof(int));
	}
	return matrix;
}

void deallocateMatrix(int** matrix, int n) {
	for (int i = 0; i < n; i++) {
		free(matrix[i]);
	}
	free(matrix);
}

int infest(int** matrix, int y, int x, int n, int m, int infestNumber) {
	int infestedCount = 0;
	for (int i = 0; i < 4; i++) {
		int newY = y, newX = x;
		switch(i) {
			case 0:
				newX--;
				break;
			case 1:
				newY--;
				break;
			case 2:
				newX++;
				break;
			case 3:
				newY++;
				break;
			default:;
		}
		if (newY >= 0 && newY < n && newX >= 0 && newX < m && matrix[newY][newX] == 0) {
			matrix[newY][newX] = infestNumber;
			infestedCount++;
		} 
	}
	return infestedCount;
}

int main(void) {
	int n, m, k;
	scanf("%d%d%d", &n, &m, &k);
	if (m == 0 || n == 0) {
		printf("0\n");
		return 0;
	}
	int** matrix = allocateMatrix(n, m);
	for (int i = 0; i < k; i++) {
		int y, x;
		scanf("%d%d", &x, &y);
		matrix[y - 1][x - 1] = 1;
	}
	int numberOfInfested = k;
	int i;
	for (i = 1; numberOfInfested < m * n; i++) {
		for (int k = 0; k < n; k++) {
			for (int j = 0; j < m; j++) {
				if (matrix[k][j] == i) {
					numberOfInfested += infest(matrix, k, j, n, m, i + 1);
				}
			}
		}
	}
	printf("%d\n", i - 1);
	deallocateMatrix(matrix, n);
	return 0;
}
