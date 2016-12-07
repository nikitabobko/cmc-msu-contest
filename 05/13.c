#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct structCell Cell;

struct structCell {
	int y;
	int x;
	int indexInBinHeap;
	int weight;
	int height;
	char isPinned;
};

Cell ***allocateMatrix(int n, int m) {
	Cell ***matrix = malloc(n * sizeof(Cell **));
	for (int i = 0; i < n; i++) {
		matrix[i] = malloc(m * sizeof(Cell *));
	}
	return matrix;
}

void swap(Cell **binHeap, int index1, int index2) {
	Cell *temp = binHeap[index1];
	binHeap[index1] = binHeap[index2];
	binHeap[index2] = temp;
	// Fix indexes
	binHeap[index1]->indexInBinHeap = index1;
	binHeap[index2]->indexInBinHeap = index2;
}

void heapify(Cell **binHeap, int index) {
	while (index != 0 
			&& binHeap[(index - 1) / 2]->weight > binHeap[index]->weight) {
		swap(binHeap, (index - 1) / 2, index);
		index = (index - 1) / 2;
	}
}

Cell *popHead(Cell **binHeap, int *howManyCellsFilled) {
	(*howManyCellsFilled)--;
	Cell *head = binHeap[0];
	if (*howManyCellsFilled == 0) return head;
	binHeap[0] = binHeap[*howManyCellsFilled];
	binHeap[0]->indexInBinHeap = 0;
	int index = 0;
	while (1) {
		int child1 = 2 * index + 1, child2 = 2 * index + 2, minChildIndex;
		if (child1 >= *howManyCellsFilled && child2 >= *howManyCellsFilled) {
			break;
		} else if (child1 >= *howManyCellsFilled || child2 >= *howManyCellsFilled) {
			minChildIndex = (child1 >= *howManyCellsFilled ? child2 : child1);
		} else {
			minChildIndex = binHeap[child1]->weight < binHeap[child2]->weight ?
				child1 : child2;
		}
		if (binHeap[index]->weight <= binHeap[minChildIndex]->weight) break;
		swap(binHeap, index, minChildIndex);
		index = minChildIndex;
	}
	return head;
}

void deallocateMatrix(Cell ***matrix, int n) {
	for (int i = 0; i < n; i++) {
		free(matrix[i]);
	}
	free(matrix);
}

int main(void) {
	int n, m, startY, startX, endX, endY;
	scanf("%d%d%d%d%d%d", &n, &m, &startY, &startX, &endY, &endX);
	Cell ***matrix = allocateMatrix(n, m);
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			matrix[i][j] = malloc(sizeof(Cell));
			matrix[i][j]->indexInBinHeap = -1;
			matrix[i][j]->weight = -1;
			matrix[i][j]->isPinned = 0;
			matrix[i][j]->y = i;
			matrix[i][j]->x = j;
			scanf("%d", &(matrix[i][j]->height));
		}
	}
	const char xOffset[] = {-1, 0, 1, 0};
	const char yOffset[] = {0, -1, 0, 1};
	matrix[startY][startX]->isPinned = 1;
	matrix[startY][startX]->weight = 0;
	int howManyCellsFilled = 0;
	Cell **binHeap = malloc(n * m * sizeof(Cell *));

	int y = startY, x = startX;
	while (!(matrix[endY][endX]->isPinned)) {
		for (int i = 0; i < 4; i++) {
			int newY = y + yOffset[i], newX = x + xOffset[i];
			if (newY >= 0 && newX >= 0 && newX < m && newY < n) {
				int oldWeight = matrix[newY][newX]->weight,
					newWeight = matrix[y][x]->weight + 
					abs(matrix[newY][newX]->height - matrix[y][x]->height);
				if (!(matrix[newY][newX]->isPinned) && (oldWeight == -1
					|| oldWeight > newWeight)) {
					matrix[newY][newX]->weight = newWeight;
					int index;
					if (matrix[newY][newX]->indexInBinHeap != -1) {
						index = matrix[newY][newX]->indexInBinHeap;
					} else {
						index = howManyCellsFilled;
						matrix[newY][newX]->indexInBinHeap = index;
						binHeap[index] = matrix[newY][newX];
						howManyCellsFilled++;
					}
					heapify(binHeap, index);
				}
			}
		}

		Cell *min = popHead(binHeap, &howManyCellsFilled);

		matrix[min->y][min->x]->isPinned = 1;
		y = min->y;
		x = min->x;
	}

	printf("%d\n", matrix[endY][endX]->weight);
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			free(matrix[i][j]);
		}
	}
	deallocateMatrix(matrix, n);
	return 0;
}
