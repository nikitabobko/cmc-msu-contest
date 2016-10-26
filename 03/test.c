#include <stdio.h>

int fun(int* arr) {
	arr[0] = 5;
	return arr[0];
}

int main(void) {
	int arr[] = {156, 5, 5};
	printf("%d\n", fun(arr));
	return 0;
}