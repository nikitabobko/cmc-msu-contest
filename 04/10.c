#include <stdio.h>
#include <string.h>

int fun(const char* str1, const char* str2) {
	int lastIndex2 = strlen(str2) - 1;
	int i, max = 0;
	for (i = 0; str1[i] != '\0'; i++) {
		if (str1[i] == str2[lastIndex2]) {
			int j;
			for (j = i - 1; j >= 0; j--) {
				if (lastIndex2 - i + j < 0 || str1[j] != str2[lastIndex2 - i + j]) {
					break;
				}
			}
			// All iterations of j loop was executed
			if (j == -1 && i + 1 > max) {
				max = i + 1;
			}
		}
	}
	return max;
}

int main(void) {
	char str1[1000001];
	char str2[1000001];
	scanf("%s%s", str1, str2);
	printf("%d %d", fun(str1, str2), fun(str2, str1));
	return 0;
}
