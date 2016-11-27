#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

typedef struct TreeNode AvlTree;

struct TreeNode {
	int key;
	int data;
	int height;
	struct TreeNode *left;
	struct TreeNode *right;
};

void addNode(AvlTree **tree, int key, int data);
AvlTree *findNode(AvlTree *tree, int key);
AvlTree *removeNode(AvlTree **tree, int key);
void fillNode(AvlTree *node, int key, int data, int height, AvlTree *left, AvlTree *right);
inline int getAvlTreeHeight(AvlTree *tree);
void rotateR(AvlTree **tree);
void rotateL(AvlTree **tree);
inline int max(int a, int b);
void ballance(AvlTree **tree);
void swapKeysAndData(AvlTree *a, AvlTree *b);
void clearTree(AvlTree *tree);

void clearTree(AvlTree *tree) {
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

inline int max(int a, int b) {
	return a > b ? a : b;
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
	left->height = max(getAvlTreeHeight(left->right), 
		getAvlTreeHeight(left->left)) + 1;
	(*tree)->height = max(getAvlTreeHeight((*tree)->right), 
		getAvlTreeHeight((*tree)->left)) + 1;

	*tree = left;
}

void rotateL(AvlTree **tree) {
	if ((*tree) == NULL || (*tree)->right == NULL) return;
	AvlTree *right = (*tree)->right;
	(*tree)->right = right->left;
	right->left = (*tree);

	// Height recalculating
	right->height = max(getAvlTreeHeight(right->left), 
		getAvlTreeHeight(right->right)) + 1;
	(*tree)->height = max(getAvlTreeHeight((*tree)->right), 
		getAvlTreeHeight((*tree)->left)) + 1;

	*tree = right;
}

inline int getAvlTreeHeight(AvlTree *tree) {
	return tree == NULL ? 0 : tree->height;
}

void fillNode(AvlTree *node, int key, int data, int height, AvlTree *left, AvlTree *right) {
	node->key = key;
	node->data = data;
	node->height = height;
	node->left = left;
	node->right = right;
}

// AvlTree *findMinNode(AvlTree *node) {
// 	if (node == NULL) return NULL;
// 	while (node->left != NULL) {
// 		node = node->left;
// 	}
// 	return node;
// }

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
		if ((*tree)->left == NULL) {
			(*tree)->left = malloc(sizeof(AvlTree));
			fillNode((*tree)->left, key, data, 1, NULL, NULL);
		} else addNode(&((*tree)->left), key, data);
		// if (getAvlTreeHeight((*tree)->left) - getAvlTreeHeight((*tree)->right) == 2) {
		// 	if (key < (*tree)->left->key) {
		// 		rotateR(tree);
		// 	} else {
		// 		rotateL(&((*tree)->left));
		// 		rotateR(tree);
		// 	}
		// }
	} else {	// key > (*tree)->key
		if ((*tree)->right == NULL) {
			(*tree)->right = malloc(sizeof(AvlTree));
			fillNode((*tree)->right, key, data, 1, NULL, NULL);
		} else addNode(&((*tree)->right), key, data);
		// if (getAvlTreeHeight((*tree)->right) - getAvlTreeHeight((*tree)->left) == 2) {
		// 	if (key > (*tree)->right->key) {
		// 		rotateL(tree);
		// 	} else {
		// 		rotateR(&((*tree)->right));
		// 		rotateL(tree);
		// 	}
		// }
	}
	ballance(tree);
	(*tree)->height = max(getAvlTreeHeight((*tree)->right), 
		getAvlTreeHeight((*tree)->left)) + 1;
}

void ballance(AvlTree **tree) {
	if ((*tree) == NULL) return; 
	if (getAvlTreeHeight((*tree)->left) - getAvlTreeHeight((*tree)->right) == 2) {
		AvlTree **left = &((*tree)->left);
		if (getAvlTreeHeight((*left)->left) > getAvlTreeHeight((*left)->right)) {
			rotateR(tree);
		} else {
			rotateL(left);
			rotateR(tree);
		}
	} else if (getAvlTreeHeight((*tree)->right) - getAvlTreeHeight((*tree)->left) == 2) {
		AvlTree **right = &((*tree)->right);
		if (getAvlTreeHeight((*right)->right) > getAvlTreeHeight((*right)->left)) {
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
				*tree = (*tree)->right;
				return removedNode;
			} else {
				AvlTree *aboveNextByKeyNode = findAboveMinNode((*tree)->right);
				AvlTree *nextByKeyNode = aboveNextByKeyNode->left;
				aboveNextByKeyNode->left = nextByKeyNode->right;
				swapKeysAndData(nextByKeyNode, (*tree));
				return nextByKeyNode;
			}
		}
	}
	if (key < (*tree)->key) {
		AvlTree *left = (*tree)->left;
		if (left == NULL) return NULL;
		if (left->key == key) {
			if (left->left == NULL || left->right == NULL) {
				removedNode = left;
				(*tree)->left = (left->left == NULL ? left->right : left->left);
			} else {
				if (left->right->left == NULL) {
					left->right->left = left->left;
					(*tree)->left = left->right;
					removedNode = left;
				} else {
					AvlTree *aboveNextByKeyNode = findAboveMinNode(left->right);
					AvlTree *nextByKeyNode = aboveNextByKeyNode->left;
					aboveNextByKeyNode->left = nextByKeyNode->right;
					swapKeysAndData(nextByKeyNode, left);
					removedNode = nextByKeyNode;
				}
			}
		} else removedNode = removeNode(&left, key);
	} else {
		AvlTree *right = (*tree)->right;
		if (right == NULL) return NULL;
		if (right->key == key) {
			if (right->left == NULL || right->right == NULL) {
				removedNode = right;
				(*tree)->right = (right->left == NULL ? right->right : right->left);
			} else {
				if (right->right->left == NULL) {
					right->right->left = right->left;
					(*tree)->right = right->right;
					removedNode = right;
				} else {
					AvlTree *aboveNextByKeyNode = findAboveMinNode(right->right);
					AvlTree *nextByKeyNode = aboveNextByKeyNode->left;
					aboveNextByKeyNode->left = nextByKeyNode->right;
					swapKeysAndData(nextByKeyNode, right);
					removedNode = nextByKeyNode;
				}
			}
		} else removedNode = removeNode(&right, key);
	}
	ballance(tree);
	(*tree)->height = max(getAvlTreeHeight((*tree)->right), 
		getAvlTreeHeight((*tree)->left)) + 1;	
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
