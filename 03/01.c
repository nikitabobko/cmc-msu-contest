#include <stdio.h>

int main(void) {
	int arr[4], i;
	for (i = 0; i < 4; i++) {
		scanf("%d", arr + i);
	}
	for(i = 1; i < 4; i++) {
		int max = arr[i] > arr[i-1] ? arr[i] : arr[i-1];
		int min = arr[i] < arr[i-1] ? arr[i] : arr[i-1];
		int modulo;
		do {
			modulo = max % min;
			if (modulo) {
				max = min;
				min = modulo;
			}
		} while(modulo);
		arr[i] = min;
	}
	printf("%d\n", arr[3]);
	return 0;
}