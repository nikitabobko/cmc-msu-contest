#include <stdio.h>
#include <string.h>

int main(void) {
	char str[102];
	scanf("%s", str);
	char marks[1000] = {};
	int i, strLen = strlen(str), count = 0;
	for (i = 0; i < strLen - 2; i++) {
		if (str[i] == '0') continue; 
		int j;
		for (j = i + 1; j < strLen - 1; j++) {
			int k;
			for (k = j + 1; k < strLen; k++) {
				int index = (str[i] - '0') * 100 + (str[j] - '0') * 10 + (str[k] - '0');
				if (!marks[index]) {
					marks[index] = 1;
					count++;
				}
			}
		}
	}
	printf("%d\n", count);
	return 0;
}
