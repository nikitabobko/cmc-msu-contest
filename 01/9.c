#include <stdio.h>

int main(void) {
	unsigned int n;
	scanf("%u", &n);
	int count = 0;
	while (n > 0) {
		count += n % 2;
		n = n >> 1;
	}
	printf("%d", count);
	return 0;
}
