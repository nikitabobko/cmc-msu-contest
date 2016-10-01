#include <stdio.h>

int main(void) {
    int n;
    scanf("%d", &n);
    int i = 0, max = -1, count = 0;
    for (i = 0; i < n; i++) {
	int cur;
	scanf("%d", &cur);
	if (max == -1 || cur > max) {
	    max = cur;
	    count = 0;
	}
	if (cur == max) count++;
    }
    printf("%d", count);
}