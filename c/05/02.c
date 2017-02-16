#include <stdio.h>
#include <stdlib.h>

int* scanMatrix(int n) {
	int* matrix = malloc(n * n * sizeof(int));
	for (int i = 0; i < n*n; i++) {
		scanf("%d", matrix + i);
	}
	return matrix;
}

void deallocateMatrix(int* matrix) {
	if (matrix != NULL) free(matrix);
}

long long matrixTrace(int* matrix, int dimension) {
	long long trace = 0;
	for (int i = 0; i < dimension; i++) {
		trace += matrix[i * dimension + i];
	}
	return trace;
}

void printMatrix(int* matrix, int dimension) {
	for (int i = 0; i < dimension; i++) {
		for (int j = 0; j < dimension; j++) {
			printf("%d ", matrix[i * dimension + j]);
		}
		printf("\n");
	}
}

int main(void) {
	int n;
	scanf("%d", &n);
	long long maxTrace = 0;
	int maxTraceMatrixDimension = 0;
	int* maxTraceMatrix = NULL; 
	for (int i = 0; i < n; i++) {
		int dimension;
		scanf("%d", &dimension);
		int* matrix = scanMatrix(dimension);
		long long trace = matrixTrace(matrix, dimension);
		if (maxTraceMatrix == NULL || trace > maxTrace) {
			deallocateMatrix(maxTraceMatrix);
			maxTrace = trace;
			maxTraceMatrixDimension = dimension;
			maxTraceMatrix = matrix;
		} else {
			deallocateMatrix(matrix);
		}
	}
	printMatrix(maxTraceMatrix, maxTraceMatrixDimension);
	deallocateMatrix(maxTraceMatrix);
	return 0;
}
