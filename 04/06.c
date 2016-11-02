#include <stdio.h>
#include <stdlib.h>

struct space
{
	int numberOfSpaces;
	int index;
};

typedef struct space _space;

/**
 * Return 0 if equals
 * 1 if s2 bigger than s1
 * -1 if s1 bigger than s2
 * compareMode - Mode of comparing 1 - compare by value and index, 2 - compare only by index
 */
int equals(_space s1, _space s2, char compareMode) {
	if (s1.numberOfSpaces == s2.numberOfSpaces && s1.index == s2.index) return 0;
	if (compareMode == 1) {
		return (s1.numberOfSpaces < s2.numberOfSpaces || 
			(s1.numberOfSpaces == s2.numberOfSpaces && s1.index < s2.index)) ? 1 : -1;
	} else {	// compareMode == 2
		return s1.index < s2.index ? 1 : -1;
	}
}

void swap(_space* s1, _space* s2) {
	_space temp = *s1;
	*s1 = *s2;
	*s2 = temp;
}

void sort(_space* x, int first, int last, char compareMode) {
	if (first < last) {
		int pivot, j, i;
		pivot = first;
		i = first;
		j = last;
		while (i < j) {
			while (equals(x[i], x[pivot], compareMode) == 1 && i < last) {
				i++;
			}
			while (equals(x[j], x[pivot], compareMode) == -1) {
				j--;
			}
			if (i < j) {
				swap(&x[i], &x[j]);
			}
		}
		_space temp = x[pivot];
		x[pivot] = x[j];
		x[j] = temp;
		sort(x, first, j - 1, compareMode);
		sort(x, j + 1, last, compareMode);
	}
}

int main(void) {
	int k;
	scanf("%d", &k);
	char str[k + 2];
	getchar();
	fgets(str, k + 2, stdin);
	int i, numberOfSpaces = 0;
	_space* spacesArr = malloc(k * sizeof(_space));
	for (i = 0; str[i] != '\n'; i++) {
		if (str[i] == ' ') {
			spacesArr[numberOfSpaces].numberOfSpaces = 0;
			spacesArr[numberOfSpaces].index = numberOfSpaces;
			while (str[i] == ' ') {
				spacesArr[numberOfSpaces].numberOfSpaces++;
				i++;
			}
			numberOfSpaces++;
			i--;
		}
	}
	sort(spacesArr, 0, numberOfSpaces-1, 1);
	// i - lenght
	for (i = k - i; i > 0; i--) {
		spacesArr[0].numberOfSpaces++;
		int j;
		for (j = 1; equals(spacesArr[j-1], spacesArr[j], 1) == -1 && j < numberOfSpaces; j++) {
			swap(&spacesArr[j-1], &spacesArr[j]);
		}
	}
	// Sort only by index
	sort(spacesArr, 0, numberOfSpaces-1, 2);
	// Now use numberOfSpaces as index
	numberOfSpaces = 0;
	for (i = 0; str[i] != '\n'; i++) {
		if (str[i] != ' ') printf("%c", str[i]);
		else {
			while (str[i] == ' ') {
				i++;
			}
			i--;
			int j;
			for (j = 0; j < spacesArr[numberOfSpaces].numberOfSpaces; j++) {
				printf(" ");
			}
			numberOfSpaces++;
		} 
	}
	return 0;
}
