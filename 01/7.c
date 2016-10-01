#include <stdio.h>
#include <math.h>

int main(void) {
	unsigned int n, k;
	scanf("%u%d", &n, &k);
	printf("%u", (n & ((unsigned int)pow(2, k)-1)));
	return 0;
}
