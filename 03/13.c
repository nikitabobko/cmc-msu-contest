#include <stdio.h>
#include <stdlib.h>

void recursive(int** arr, int index, int startFrom, int max, int k) {
	int i;
	if (index == k) {
		// Just print sequence
		for (i = 0; i < k; i++) {
			printf("%d ", (*arr)[i]);
		}
		printf("\n");
		return;
	}
	for (i = startFrom; i <= max; i++) {
		(*arr)[index] = i;
		recursive(arr, index + 1, i + 1, max + 1, k);
	}
}

int main(void) {
	int n, k;
	scanf("%d", &n);
	scanf("%d", &k);
	int* arr = malloc(k * sizeof(int));
	recursive(&arr, 0, 0, n - k, k);
	return 0;
}
