#include <stdio.h>
#include <math.h>

int main(void) {
	unsigned long n;
	int i, k;
	scanf("%lu %d", &n, &k);
	for (i = 0; i < k; i++) {
		if (n % 2 == 0) n /= 2;
		else n = (n-1)/2 + pow(2, 31);
	}
	printf("%lu", n);
	return 0;
}
