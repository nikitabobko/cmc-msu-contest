#include <stdio.h>

void recursive(void) {
	int n;
	scanf("%d", &n);
	if (n) {
		recursive();
		printf("%d ", n);
	}
}

int main(void) {
	recursive();
	return 0;
}