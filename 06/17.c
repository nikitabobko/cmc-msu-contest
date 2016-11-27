#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct TreeNode AvlTree;

struct TreeNode {
	int key;
	int data;
	int height;
	struct TreeNode *left;
	struct TreeNode *right;
};

void addNode(AvlTree **tree, int key, int data);
AvlTree *nextNodeByKey(AvlTree *node);
AvlTree *findNode(AvlTree *tree, int key);
AvlTree *findMinNode(AvlTree *node);
AvlTree *removeNode(AvlTree **tree, int key);
void fillNode(AvlTree *node, int key, int data, int height, AvlTree *left, AvlTree *right);
int inline getAvlTreeHeight(AvlTree *tree);
void rotateR(AvlTree **tree);
void rotateL(AvlTree **tree);
int inline max(int a, int b);
void ballance(AvlTree **tree);

int inline max(int a, int b) {
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

int inline getAvlTreeHeight(AvlTree *tree) {
	return tree == NULL ? 0 : tree->height;
}

void fillNode(AvlTree *node, int key, int data, int height, AvlTree *left, AvlTree *right) {
	node->key = key;
	node->data = data;
	node->height = height;
	node->left = left;
	node->right = right;
}

AvlTree *findMinNode(AvlTree *node) {
	if (node == NULL) return NULL;
	while (node->left != NULL) {
		node = node->left;
	}
	return node;
}

void addNode(AvlTree **tree, int key, int data) {
	if ((*tree) == NULL) {
		(*tree) = malloc(sizeof(AvlTree));
		fillNode((*tree), key, data, 1, NULL, NULL);
		return;
	}
	if (key < (*tree)->key) {
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

AvlTree *removeNode(AvlTree **tree, int key) {
	if ((*tree) == NULL) return NULL;
	if ((*tree)->key == key) {
		if ((*tree)->left == NULL || (*tree)->right == NULL) {

		} else {

		}	

		// AvlTree *node = *tree;
		// *tree = NULL;
		// return node;
	}
	AvlTree *node;
	if (key < (*tree)->key) {
		if ((*tree)->left == NULL) return NULL;
		node = removeNode(&((*tree)->left), key);
	} else {
		if ((*tree)->right == NULL) return NULL;
		node = removeNode(&((*tree)->right), key);
	}
	ballance(tree);
	return node;
}

AvlTree *nextNodeByKey(AvlTree *node) {
	if (node == NULL) return NULL;
	if (node->right != NULL) {
		return findMinNode(node->right);
	}

}

int main(void) {
	
	return 0;
}
