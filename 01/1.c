#include <stdio.h>

int main(void) {
    int n;
    scanf("%d", &n);
    if (n == 1) {
	printf("0");
	return 0;
    }
    int i, min = -1, max = -1;
    for (i = 0; i < n; i++) {
	int cur;
	scanf("%d", &cur);
	if (min == -1 || cur < min) min = cur;
	if (max == -1 || cur > max) max = cur;
    }
    printf("%d", max-min);
}
