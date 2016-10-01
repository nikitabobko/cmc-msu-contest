#include <stdio.h>

int main(void) {
	int x, y;
	scanf("%d%d", &x, &y);
	int t = x;
	x = x > y ? x : y;
	y = t < y ? t : y;
	printf("%d", 2 * x - (x - y) % 2);
	return 0;
}
