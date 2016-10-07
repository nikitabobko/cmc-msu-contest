#include <stdio.h>

int main(void) {
	int n, i;
	scanf("%d", &n);
	int arr1[n];
	int arr2[n];
	for (i = 0; i < n; i++) {
		scanf("%d", arr1 + i);
		arr2[i] = arr1[i];
	}
	for (i = 0; i < n; i++) {
		arr2[i] = arr1[arr1[i]-1];
	}
	for (i = 0; i < n; i++) {
		printf("%d ", arr2[arr1[i]-1]);
	}
	return 0;
}