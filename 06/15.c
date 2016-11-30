#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define NAME_LENGTH 101

typedef struct TreeNode AvlTree;

struct TreeNode {
	char *name;
	unsigned ip;
	int height;
	struct TreeNode *left;
	struct TreeNode *right;
};

char compareNames(char* const a, char* const b);
void addNode(AvlTree **tree, char *name, unsigned ip);
AvlTree *findNode(AvlTree *tree, char *name);
void fillNode(AvlTree *node, char *name, unsigned ip, int height, AvlTree *left, AvlTree *right);
void rotateR(AvlTree **tree);
void rotateL(AvlTree **tree);
void ballance(AvlTree **tree);
void swapNamesAndIp(AvlTree *a, AvlTree *b);
void clearTree(AvlTree *tree);
int getMaxHeight(AvlTree *a, AvlTree *b);
int getAvlTreeHeight(AvlTree *tree);

char compareNames(char* const a, char* const b) {
	for (int i = 0; i < NAME_LENGTH; i++) {
		if (a[i] == '\0' && b[i] == '\0') break;
		if (a[i] > b[i]) return -1;
		else if (b[i] > a[i]) return 1;
	}
	return 0;
}

int getMaxHeight(AvlTree *a, AvlTree *b) {
	int aHeight = (a == NULL ? 0 : a->height);
	int bHeight = (b == NULL ? 0 : b->height);
	return aHeight > bHeight ? aHeight : bHeight;
}

void clearTree(AvlTree *tree) {
	if (tree == NULL) return;
	if (tree->left != NULL) {
		clearTree(tree->left);
	}
	if (tree->right != NULL) {
		clearTree(tree->right);
	}
	free(tree->name);
	free(tree);
}

void swapNamesAndIp(AvlTree *a, AvlTree *b) {
	if (a == NULL || b == NULL) return;
	char *nameA = a->name;
	int ipA = a->ip;
	a->name = b->name;
	a->ip = b->ip;
	b->name = nameA;
	b->ip = ipA;
}

AvlTree *findNode(AvlTree *tree, char* name) {
	char comp = compareNames(name, tree->name);
	while (tree != NULL && comp != 0) {
		if (comp == 1) {
			tree = tree->left;
		} else {
			tree = tree->right;	
		}
		if (tree != NULL) comp = compareNames(name, tree->name);
	}
	return tree;
}

void rotateR(AvlTree **tree) {
	if ((*tree) == NULL || (*tree)->left == NULL) return;
	AvlTree *left = (*tree)->left;
	(*tree)->left = left->right;
	left->right = (*tree);

	// Height recalculating
	(*tree)->height = getMaxHeight((*tree)->right, (*tree)->left) + 1;
	left->height = getMaxHeight(left->right, left->left) + 1;

	*tree = left;
}

void rotateL(AvlTree **tree) {
	if ((*tree) == NULL || (*tree)->right == NULL) return;
	AvlTree *right = (*tree)->right;
	(*tree)->right = right->left;
	right->left = (*tree);

	// Height recalculating
	(*tree)->height = getMaxHeight((*tree)->right, (*tree)->left) + 1;
	right->height = getMaxHeight(right->left, right->right) + 1;

	*tree = right;
}

void fillNode(AvlTree *node, char *name, unsigned ip, int height, AvlTree *left, AvlTree *right) {
	node->name = name;
	node->ip = ip;
	node->height = height;
	node->left = left;
	node->right = right;
}

void addNode(AvlTree **tree, char *name, unsigned ip) {
	if ((*tree) == NULL) {
		(*tree) = malloc(sizeof(AvlTree));
		fillNode((*tree), name, ip, 1, NULL, NULL);
		return;
	}
	char comp = compareNames((*tree)->name, name);
	if (comp == 0) {
		(*tree)->ip = ip;
		return;
	} else if (comp == -1) {
		if ((*tree)->left == NULL) {
			(*tree)->left = malloc(sizeof(AvlTree));
			fillNode((*tree)->left, name, ip, 1, NULL, NULL);
		} else {
			addNode(&((*tree)->left), name, ip);
		}
	} else {	// comp == 1
		if ((*tree)->right == NULL) {
			(*tree)->right = malloc(sizeof(AvlTree));
			fillNode((*tree)->right, name, ip, 1, NULL, NULL);
		} else {
			addNode(&((*tree)->right), name, ip);	
		}
	}
	ballance(tree);
	(*tree)->height = getMaxHeight((*tree)->right, (*tree)->left) + 1;
}

int getAvlTreeHeight(AvlTree *tree) {
	return (tree == NULL ? 0 : tree->height);
}

void ballance(AvlTree **tree) {
	if ((*tree) == NULL) return;
	if (getAvlTreeHeight((*tree)->left) - getAvlTreeHeight((*tree)->right) == 2) {
		AvlTree **left = &((*tree)->left);
		if (getAvlTreeHeight((*left)->left) >= getAvlTreeHeight((*left)->right)) {
			rotateR(tree);
		} else {
			rotateL(left);
			rotateR(tree);
		}
	} else if (getAvlTreeHeight((*tree)->right) - getAvlTreeHeight((*tree)->left) == 2) {
		AvlTree **right = &((*tree)->right);
		if (getAvlTreeHeight((*right)->right) >= getAvlTreeHeight((*right)->left)) {
			rotateL(tree);
		} else {
			rotateR(right);
			rotateL(tree);
		}
	}
}

int main(void) {
	FILE *in = fopen("input.txt", "r");
	FILE *out = fopen("output.txt", "w");

	AvlTree *tree = NULL;
	int n;
	fscanf(in, "%d", &n);
	for (int i = 0; i < n; i++) {
		char *name = malloc(NAME_LENGTH);
		unsigned ip;
		fscanf(in, "%s %u", name, &ip);
		addNode(&tree, name, ip);
	}
	fscanf(in, "%d", &n);
	char *name = malloc(NAME_LENGTH);
	for (int i = 0; i < n; i++) {
		fscanf(in, "%s", name);
		AvlTree *node = findNode(tree, name);
		if (node != NULL) fprintf(out, "%u\n", node->ip);
		else fprintf(out, "-1\n");
	}
	free(name);

	fclose(out);
	fclose(in);	
	return 0;
}
