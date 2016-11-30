#include <stdio.h>
#include <stdlib.h>

struct structList {
	int value;
	struct structList* next;
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
	*list = newHead;
}

void operation(List **list, int a, int b) {
	if (a == (*list)->value) return;
	List *start = *list;
	while (start->next->value != a) {
		start = start->next;
	}
	List *end = start->next;
	while (end->value != b) {
		end = end->next;
	}
	List *newHead = start->next;
	start->next = end->next;
	end->next = *list;
	*list = newHead;
}

int main(void) {
	FILE *in = fopen("input.txt", "r");
	FILE *out = fopen("output.txt", "w");

	List *list = NULL;
	int n, m;
	fscanf(in, "%d %d", &n, &m);
	for (int i = n; i >= 1; i--) {
		add(&list, i);
	}
	for (int i = 0; i < m; i++) {
		int a, b;
		fscanf(in, "%d %d", &a, &b);
		operation(&list, a, b);
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
