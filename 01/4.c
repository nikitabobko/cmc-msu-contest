#include <stdio.h>

int main(void) {
	int n;
	scanf("%d", &n);
	int i, prev, curCount = 1, maxCount = 1;
	if (n == 1) {
		printf("1");
		return 0;
	}
	scanf("%d", &prev);
	for (i = 1; i < n; i++) {
		int cur;
		scanf("%d", &cur);
		if (cur > prev) curCount++;
		else {
			if (curCount > maxCount) maxCount = curCount;
			curCount = 1;
		}
		prev = cur;
	}
	printf("%d", maxCount > curCount ? maxCount : curCount);
	return 0;
}
