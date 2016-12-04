#include <stdio.h>
#include <stdlib.h>

typedef struct structList List;

struct structList {
	int val;
	// Inverse index
	int invIndex;
	List *next;
	List *previous;
};

void add(List **list, int val, int invIndex) {
	if (list == NULL) return;
	List *oldHead = *list;
	*list = malloc(sizeof(List));
	(*list)->val = val;
	(*list)->invIndex = invIndex;
	(*list)->next = oldHead;
	if (oldHead != NULL) oldHead->previous = *list;
	(*list)->previous = NULL;
}

void quickSort(List *left, List *right) {
	if (left == NULL || right == NULL || left < right) return;
	int pivot = right->val;
	List *i = left, *j = right;
	do {
		while (i->val < pivot && i != right) {
			i = i->next;
		}
		while (j->val > pivot && j != left) {
			j = j->previous;
		}
		if (i->invIndex >= j->invIndex) {
			int t = i->val;
			i->val = j->val;
			j->val = t;
			i = i->next;
			j = j->previous;
		}
	} while (i != NULL && j != NULL && i->invIndex > j->invIndex);
	if (i != right) {
		quickSort(i, right);
	}
	if (j != left) {
		quickSort(left, j);
	}
}

int main(void) {
	FILE *in = fopen("input.txt", "r");
	FILE *out = fopen("output.txt", "w");

	List *left = NULL;
	List *right = NULL;
	for(int i = 0; 1; i++) { 
		int curNum = 0;
		if (fscanf(in, "%d", &curNum) == EOF) break;
		add(&left, curNum, i);
		if (right == NULL) right = left;
	}
	quickSort(left, right);
	while (left != NULL) {
		fprintf(out, "%d ", left->val);
		right = left;
		left = left->next;
		free(right);
	}

	fclose(out);
	fclose(in);
	return 0;
}
