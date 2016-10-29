#include <stdio.h>

void recursive(void) {
	int x, y;
	scanf("%d", &x);
	if (x) {
		printf("%d ", x);
		scanf("%d", &y);
		if (y) {
			recursive();
			printf("%d ", y);
		}
	}
}

int main(void) {
	recursive();
	return 0;
}