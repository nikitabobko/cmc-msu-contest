#include <stdio.h>

int isPrime(int n) {
	if (n == 1) return 0;
	int i;
	for (i = 2; i < n; i++) {
		if (n % i == 0) return 0;
	}
	return 1;
}

int main(void) {
	int n;
	scanf("%d", &n);
	for (; 1; n++) {
		if (isPrime(n)) {
			printf("%d\n", n);
			return 0;
		}
	}
	return 1;
}