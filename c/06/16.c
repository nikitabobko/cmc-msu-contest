#include <stdio.h>
#include <stdlib.h>

typedef struct structList List;
typedef struct structListList ListList;

struct structListList {
	List *list;
	ListList *next;
};

struct structList {
	int val;
	List *next;
};

List *merge(List *l1, List *l2) {
	if (l1 == NULL) return l2;
	if (l2 == NULL) return l1;
	List *cur1 = l1, *cur2 = l2, *newHead = NULL, *end = NULL;
	while (cur1 != NULL && cur2 != NULL) {
		List **a = newHead == NULL ? &newHead : &(end->next);
		*a = cur1->val < cur2->val ? cur1 : cur2;
		end = *a;
		if (cur1->val < cur2->val) cur1 = cur1->next;
		else cur2 = cur2->next;
		end->next = NULL;
	}
	end->next = cur1 != NULL ? cur1 : cur2;
	return newHead;
}

void add(ListList **ll, List *l) {
	if (ll == NULL) return;
	ListList *oldHead = *ll;
	*ll = malloc(sizeof(ListList));
	(*ll)->list = l;
	(*ll)->next = oldHead;
}

int main(void) {
	FILE *in = fopen("input.txt", "r");
	FILE *out = fopen("output.txt", "w");

	ListList *ll = NULL;
	while (1) { 
		int curNum = 0;
		if (fscanf(in, "%d", &curNum) == EOF) break;
		List *l = malloc(sizeof(List));
		l->val = curNum;
		l->next = NULL;
		add(&ll, l);
	}
	// Merge sort
	while (ll->next != NULL) {
		ListList *cur = ll;
		while (cur != NULL && cur->next != NULL) {
			ListList *next = cur->next;
			cur->list = merge(cur->list, next->list);
			cur->next = next->next;
			cur = cur->next;
			free(next);
		}
	}
	List *l = ll->list;
	// Print
	while (l != NULL) {
		fprintf(out, "%d ", l->val);
		List *temp = l;
		l = l->next;
		free(temp);
	}

	fclose(out);
	fclose(in);
	return 0;
}
