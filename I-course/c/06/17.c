#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct TreeNode AvlTree;

struct TreeNode {
	int key;
	int data;
	int height;
	AvlTree *left;
	AvlTree *right;
};

void addNode(AvlTree **tree, int key, int data);
AvlTree *findNode(AvlTree *tree, int key);
AvlTree *removeNode(AvlTree **tree, int key);
void fillNode(AvlTree *node, int key, int data, int height, AvlTree *left, AvlTree *right);
void rotateR(AvlTree **tree);
void rotateL(AvlTree **tree);
void ballance(AvlTree **tree);
void swapKeysAndData(AvlTree *a, AvlTree *b);
void clearTree(AvlTree *tree);
int recalculateHeight(AvlTree *tree);
int getMaxHeight(AvlTree *a, AvlTree *b);
int getAvlTreeHeight(AvlTree *tree);

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
	free(tree);
}

void swapKeysAndData(AvlTree *a, AvlTree *b) {
	if (a == NULL || b == NULL) return;
	int keyA = a->key;
	int dataA = a->data;
	a->key = b->key;
	a->data = b->data;
	b->key = keyA;
	b->data = dataA;
}

AvlTree *findNode(AvlTree *tree, int key) {
	while (tree != NULL && tree->key != key) {
		if (key < tree->key) {
			tree = tree->left;
		} else {
			tree = tree->right;
		}
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

void fillNode(AvlTree *node, int key, int data, int height, AvlTree *left, AvlTree *right) {
	node->key = key;
	node->data = data;
	node->height = height;
	node->left = left;
	node->right = right;
}

void addNode(AvlTree **tree, int key, int data) {
	if ((*tree) == NULL) {
		(*tree) = malloc(sizeof(AvlTree));
		fillNode((*tree), key, data, 1, NULL, NULL);
		return;
	}
	if ((*tree)->key == key) {
		(*tree)->data = data;
		return;
	} else if (key < (*tree)->key) {
			addNode(&((*tree)->left), key, data);
	} else {	// key > (*tree)->key
		addNode(&((*tree)->right), key, data);	
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

AvlTree *findAboveMinNode(AvlTree *node) {
	if (node == NULL || node->left == NULL) return NULL;
	while (node->left->left != NULL) {
		node = node->left;
	}
	return node;
}

int recalculateHeight(AvlTree *tree) {
	if (tree == NULL) return 0;
	if (tree->left != NULL) {
		recalculateHeight(tree->left);
	}
	if (tree->right != NULL) {
		recalculateHeight(tree->right);
	}
	tree->height = getMaxHeight(tree->left, tree->right) + 1;
	return tree->height;
}

AvlTree *removeNode(AvlTree **tree, int key) {
	if ((*tree) == NULL) return NULL;
	AvlTree *removedNode;
	if ((*tree)->key == key) {
		removedNode = *tree;
		if ((*tree)->left == NULL || (*tree)->right == NULL) {
			*tree = ((*tree)->left == NULL ? (*tree)->right : (*tree)->left);
			return removedNode;
		} else {
			if ((*tree)->right->left == NULL) {
				(*tree)->right->left = (*tree)->left;
				// Height recalculation
				(*tree)->right->height = getMaxHeight((*tree)->left, (*tree)->right) + 1;
				*tree = (*tree)->right;
			} else {
				AvlTree *aboveNextByKeyNode = findAboveMinNode((*tree)->right);
				AvlTree *nextByKeyNode = aboveNextByKeyNode->left;
				aboveNextByKeyNode->left = nextByKeyNode->right;
				swapKeysAndData(nextByKeyNode, (*tree));
				// Height recalculation
				recalculateHeight((*tree)->right->left);
				(*tree)->right->height = getMaxHeight((*tree)->right->left, (*tree)->right->right) + 1;
				(*tree)->height = getMaxHeight((*tree)->right, (*tree)->left) + 1;
				removedNode = nextByKeyNode;
			}
		}
	} else if (key < (*tree)->key) {
		removedNode = removeNode(&((*tree)->left), key);
	} else {
		removedNode = removeNode(&((*tree)->right), key);
	}
	ballance(tree);
	(*tree)->height = getMaxHeight((*tree)->right, (*tree)->left) + 1;
	return removedNode;
}

int main(void) {
	AvlTree *tree = NULL;
	while(1) {
		char command[2];
		scanf("%s", command);
		if (command[0] == 'F') break; 
		int key, data;
		AvlTree *foundNode = NULL;
		scanf("%d", &key);
		switch (command[0]) {
			case 'A':	// Add
				scanf("%d", &data);
				addNode(&tree, key, data);
				break;
			case 'S':	// Search
				foundNode = findNode(tree, key);
				if (foundNode != NULL) 
					printf("%d %d\n", key, foundNode->data);
				break;
			case 'D':	// Delete
				free(removeNode(&tree, key));
				break;
			default:
				assert(0);
		}
	}
	clearTree(tree);
	return 0;
}
