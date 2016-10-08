#include <stdio.h>

int main(void){
	int n, i;
	scanf("%d", &n);
	int a[n];
	for (i = 0; i < n; i++) scanf("%d", a + i);
	int m;
	scanf("%d", &m);
	int x[m];
	for (i = m-1; i >= 0; i--) scanf("%d", x + i);
	for (i = 0; i < m; i++) {
		int j, summ = 0;
		int powOfX = 1;
		for (j = 0; j < n; j++, powOfX *= x[i]) {
			summ += a[j] * powOfX;
		}
		printf("%d ", summ);
	}
	return 0;
}
