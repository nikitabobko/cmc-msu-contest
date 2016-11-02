#include <stdio.h>
#include <string.h>

int main(void) {
	char str[10001];
	scanf("%s", str);
	int t, lenght = strlen(str);
	char is2 = 0;
	for (t = 1; t < lenght; t++) {
		int k;
		for (k = 0; k < t; k++) {
			int i;
			for (i = k; i < lenght; i += t) {
				if (str[i] != str[k]) {
					goto label;
				}
			}
		}
		label:
		// Last iterations of k loop was executed
		if (k >= t) {
			printf("%d ", t);
			if (t == 2) is2 = 1;
		}
	}
	printf("%d", lenght);
	return 0;
}
