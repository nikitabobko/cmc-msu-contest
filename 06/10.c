#include <stdio.h>
#include <stdlib.h>

struct structList {
	int value;
	struct structList *next;
	struct structList *previous;
};

typedef struct structList List;

void add(List **list, int value) {
	if ((*list) == NULL) {
		*list = malloc(sizeof(List));
		(*list)->next = NULL;
		(*list)->value = value;
		return;
	}
	List *newHead = malloc(sizeof(List));
	newHead->value = value;
	newHead->next = *list;
	newHead->previous = NULL;
	(*list)->previous = newHead;
	*list = newHead;
}

int main(void) {
	FILE *in = fopen("input.txt", "r");
	FILE *out = fopen("output.txt", "w");

	List *list = NULL;
	int n, m;
	fscanf(in, "%d %d", &n, &m);
	List **listArr = malloc(n * sizeof(List *));
	for (int i = n; i >= 1; i--) {
		add(&list, i);
		listArr[i - 1] = list;
	}
	for (int i = 0; i < m; i++) {
		int a, b;
		fscanf(in, "%d %d", &a, &b);
		a--;
		b--;
		if (listArr[a]->previous == NULL) continue;
		listArr[a]->previous->next = listArr[b]->next;
		if (listArr[b]->next != NULL) 
			listArr[b]->next->previous = listArr[a]->previous;

		listArr[b]->next = list;
		list->previous = listArr[b];

		listArr[a]->previous = NULL;
		list = listArr[a];
	}
	while (list != NULL) {
		fprintf(out, "%d ", list->value);
		List *temp = list;
		list = list->next;
		free(temp);
	}

	fclose(out);
	fclose(in);
	return 0;
}
