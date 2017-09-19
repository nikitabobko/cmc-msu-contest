#include <stdio.h>
#include <stdlib.h>

int** allocateMatrix(int n) {
	int** matrix = malloc(n * sizeof(int*));
	for (int i = 0; i < n; i++) {
		matrix[i] = calloc(n, sizeof(int));
	}
	return matrix;
}

void deallocateMatrix(int** matrix, int n) {
	for (int i = 0; i < n; i++) {
		free(matrix[i]);
	}
	free(matrix);
}

int fill(int** matrix, int y, int x, int fillWith, int n) {
	static const int xOffset[8] = {-2, -1, 1, 2, 2, 1, -1, -2};
	static const int yOffset[8] = {-1, -2, -2, -1, 1, 2, 2, 1};
	int filledCount = 0;
	for (int i = 0; i < 8; i++) {
		int newY = y + yOffset[i], newX = x + xOffset[i];
		if (newX >= 0 && newY >= 0 && newX < n && newY < n && matrix[newY][newX] == 0) {
			matrix[newY][newX] = fillWith;
			filledCount++;
		}
	}
	return filledCount;
}

int main(void) {
	
	int n, oldX, oldY, newX, newY;
	scanf("%d%d%d%d%d", &n, &oldY, &oldX, &newY, &newX);
	oldX--;
	oldY--;
	newX--;
	newY--;
	int** matrix = allocateMatrix(n);
	matrix[oldY][oldX] = 1;
	for (int k = 1; matrix[newY][newX] == 0; k++) {
		int filledCount = 0;
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				if (matrix[i][j] == k) {
					filledCount += fill(matrix, i, j, k + 1, n);
				}
			}
		}
		if (filledCount == 0) break;

	}
	printf("%d\n", matrix[newY][newX] - 1);
	deallocateMatrix(matrix, n);
	return 0;
}
