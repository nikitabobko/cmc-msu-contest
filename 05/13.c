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

LinkedList *addToList(LinkedList *linkedList, int y, int x, int** const graph) {
	LinkedList* curElement = linkedList;
	for (;curElement != NULL && curElement->next != NULL 
		&& graph[curElement->next->y][curElement->next->x] < graph[y][x]; 
		curElement = curElement->next);
	LinkedList* insert = malloc(sizeof(LinkedList));
	insert->x = x;
	insert->y = y;
	if (curElement == NULL) {	
		insert->previous = NULL;
		insert->next = NULL;
		return insert;
	} else {
		if (curElement->next == NULL) {
			curElement->next = insert;
			insert->previous = curElement;
			insert->next = NULL;
		} else {
			insert->previous = curElement;
			insert->next = curElement->next;
			insert->next->previous = insert;
			curElement->next = insert;
		}
		return linkedList;
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
			int newY = y + yOffset[i], newX = x + xOffset[i];
			if (newY >= 0 && newX >= 0 && newX < m && newY < n
				&& (abs(endY - newY) < abs(endY - y) || abs(endX - newX) < abs(endX - x))
				&& !isPinned[newY][newX] && (graph[newY][newX] == 0
				|| graph[newY][newX] > graph[y][x] + abs(map[newY][newX] - map[y][x]))) {
				graph[newY][newX] = graph[y][x] + abs(map[newY][newX] - map[y][x]);
				linkedList = addToList(linkedList, newY, newX, graph);
			}
		}
		// LinkedList *minElement = NULL, *curElement;
		// for (curElement = linkedList; curElement != NULL; curElement = curElement->next) {
		// 	if (minElement == NULL || 
		// 		graph[curElement->y][curElement->x] < graph[minElement->y][minElement->x]){
		// 		minElement = curElement;
		// 	}
		// }
		isPinned[linkedList->y][linkedList->x] = 1;
		y = linkedList->y;
		x = linkedList->x;
		linkedList = removeFromList(linkedList, linkedList);
	}

	printf("%d\n", graph[endY][endX]);
	deallocateMatrix((void **) isPinned, n);
	deallocateMatrix((void **) graph, n);
	deallocateMatrix((void **) map, n);
	return 0;
}
