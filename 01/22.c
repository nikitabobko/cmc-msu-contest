#include <stdio.h>

int main(void) {
	long long int x, y;
	scanf("%lld%lld", &x, &y);
	if (x < y) {
		int t = x;
		x = y;
		y = t;
	}
	printf("%lld", 2 * x - ((x - y) % 2));
	return 0;
}
