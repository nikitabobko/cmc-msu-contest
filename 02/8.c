#include <stdio.h>

int main(void){
	int n, i, count = 0;
	scanf("%d", &n);
	int a[n];
	for (i = 0; i < n; i++) scanf("%d", a + i);
	for (i = 0; i < n-2; i++) {
		int j;
		for (j = i + 2; j < n; j += 2) if ((a[i] + a[j]) / 2 == a[(i + j) / 2]) count++;
	}
	printf("%d\n", count);
	return 0;
}