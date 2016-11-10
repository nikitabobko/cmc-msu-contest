#include <stdio.h>

int main(void) {
	int n, m, i;
	scanf("%d", &n);
	int arr1[n];
	for (i = 0; i < n; i++) scanf("%d", arr1 + i);
	scanf("%d", &m);
	int arr2[m];
	for (i = 0; i < m; i++) scanf("%d", arr2 + i);
	int max = n > m ? n : m;
	for (i = 0; i < max; i++) {
		if (i < n) printf("%d ", arr1[i]);
		if (i < m) printf("%d ", arr2[i]);
	}
	return 0;
}