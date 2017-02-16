#include <stdio.h>
#include <stdlib.h>

int main(void) {
	int sizeOfArr = 1, howManyFilled = 0;
	int* arr = malloc(sizeOfArr * sizeof(int));
	for (int i = 0; 1; i++) {
		int n;
		scanf("%d", &n);
		if (n == 0) break;
		if (i % 2 == 0) {
			printf("%d ", n);
		} else {	
			if (howManyFilled == sizeOfArr) {
				sizeOfArr *= 2;
				arr = realloc(arr, sizeOfArr * sizeof(int));
			}
			arr[howManyFilled] = n;
			howManyFilled++;
		}
	}
	for (int i = 0; i < howManyFilled; i++) {
		printf("%d ", arr[i]);
	}
	free(arr);
	return 0;
}
