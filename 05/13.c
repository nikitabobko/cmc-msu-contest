#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct list
{
	int y;
	int x;
	struct list *next;
	struct list *previous;
};

typedef struct list LinkedList;

int **allocateMatrix(int n, int m, const char isMalloc) {
	int **matrix = malloc(n * sizeof(int*));
	for (int i = 0; i < n; i++) {
		if (isMalloc) matrix[i] = malloc(m * sizeof(int));
		else matrix[i] = calloc(m, sizeof(int));
	}
	return matrix;
}

LinkedList *addToList(LinkedList *linkedList, int y, int x) {
	LinkedList *headBackup = linkedList;
	while (linkedList != NULL && linkedList->next != NULL) {
		linkedList = linkedList->next;
	}
	if (linkedList == NULL) {
		linkedList = malloc(sizeof(LinkedList));
		linkedList->y = y;
		linkedList->x = x;
		linkedList->next = NULL;
		linkedList->previous = NULL;
		return linkedList;
	} else {
		linkedList->next = malloc(sizeof(LinkedList));
		linkedList->next->y = y;
		linkedList->next->x = x;
		linkedList->next->previous = linkedList;
		linkedList->next->next = NULL;
		return headBackup;
	}
}

LinkedList *removeFromList(LinkedList *linkedList, LinkedList *objToRemove) {
	LinkedList *newHead = linkedList;
	if (objToRemove->previous != NULL) objToRemove->previous->next = objToRemove->next;
	else newHead = objToRemove->next;
	if (objToRemove->next != NULL) objToRemove->next->previous = objToRemove->previous;
	free(objToRemove);
	return newHead;
}

char **allocateCharMatrix(int n, int m) {
	char **matrix = malloc(n * sizeof(char*));
	for (int i = 0; i < n; i++) {
		matrix[i] = calloc(m, 1);
	}
	return matrix;
}

void deallocateMatrix(void **matrix, int n) {
	for (int i = 0; i < n; i++) {
		free(matrix[i]);
	}
	free(matrix);
}

int main(void) {
	int n, m, startY, startX, endX, endY;
	scanf("%d%d%d%d%d%d", &n, &m, &startY, &startX, &endY, &endX);
	int** map = allocateMatrix(n, m, 1);
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			scanf("%d", &(map[i][j]));
		}
	}
	int **graph = allocateMatrix(n, m, 0);
	char **isPinned = allocateCharMatrix(n, m);
	const char xOffset[] = {-1, 0, 1, 0};
	const char yOffset[] = {0, -1, 0, 1};
	isPinned[startY][startX] = 1;
	LinkedList *linkedList = NULL;

	int y = startY, x = startX;
	while (!isPinned[endY][endX]) {
		for (int i = 0; i < 4; i++) {
			int newYOffset = y + yOffset[i], newXOffset = x + xOffset[i];
			if (newYOffset >= 0 && newXOffset >= 0 && newXOffset < m && newYOffset < n
				&& !isPinned[newYOffset][newXOffset] && (graph[newYOffset][newXOffset] == 0
 				|| graph[newYOffset][newXOffset]>graph[y][x]+abs(map[newYOffset][newXOffset]-map[y][x]))){
				graph[newYOffset][newXOffset] = graph[y][x]+abs(map[newYOffset][newXOffset]-map[y][x]);
				linkedList = addToList(linkedList, newYOffset, newXOffset);
			}
		}
		LinkedList *minElement = NULL, *curElement;
		for (curElement = linkedList; curElement != NULL; curElement = curElement->next) {
			if (minElement == NULL || 
				graph[curElement->y][curElement->x] < graph[minElement->y][minElement->x]){
				minElement = curElement;
			}
		}
		isPinned[minElement->y][minElement->x] = 1;
		y = minElement->y;
		x = minElement->x;
		linkedList = removeFromList(linkedList, minElement);
	}

	printf("%d\n", graph[endY][endX]);
	deallocateMatrix((void **) isPinned, n);
	deallocateMatrix((void **) graph, n);
	deallocateMatrix((void **) map, n);
	return 0;
}
