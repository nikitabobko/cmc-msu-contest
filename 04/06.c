#include <stdio.h>

int main(void) {
	int k;
	scanf("%d", &k);
	char str[k + 2];
	getchar();
	fgets(str, k + 2, stdin);
	// Count number of spaces
	int i, numberOfSpaces = 0;
	for (i = 0; str[i] != '\n'; i++) {
		if (str[i] == ' ') numberOfSpaces++;
	}
	k -= i;
	int modulo = k % numberOfSpaces;
	k /= numberOfSpaces;
	for (i = 0; str[i] != '\n'; i++) {
		printf("%c", str[i]);
		if (str[i] == ' ') {
			int j;
			for (j = 0; j < k + (modulo > 0 ? 1 : 0); j++) {
				printf(" ");
			}
			if (modulo != 0) modulo--;
		}  
	}
	return 0;
}
