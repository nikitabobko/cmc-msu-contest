#include <stdio.h>
#include <stdlib.h>
#define MAX_LINE_LENGHT 1000001

char* format(int k, char* const str, int startIndex, int endIndex) {
	/* Algoritmh from 04/06 exercise */
	char* result = malloc(k + 1);
	// Count number of spaces
	int i, numberOfSpaces = 0;
	for (i = startIndex; i <= endIndex; i++) {
		if (str[i] == ' ') numberOfSpaces++;
	}
	const int kBackup = k;
	k -= (endIndex - startIndex + 1);
	if (numberOfSpaces == 0) {
		numberOfSpaces++;
		endIndex++;
	}
	int modulo = k % numberOfSpaces;
	k /= numberOfSpaces;
	int j = 0;
	for (i = startIndex; i <= endIndex; i++) {
		result[j] = str[i] == '\n' ? ' ' : str[i];
		if (result[j] == ' ') {
			j++;
			int g;
			for (g = 0; g < k + (modulo > 0 ? 1 : 0); g++) {
				result[j] = ' ';
				j++;
			}
			if (modulo != 0) modulo--;
		} else j++;
	}
	result[kBackup] = '\0';
	return result;
}

int main(void) {
	int k;
	scanf("%d", &k);
	// Scan line
	char str[MAX_LINE_LENGHT];
	getchar();
	fgets(str, MAX_LINE_LENGHT, stdin);

	int i, lastSpaceIndex = -1, lastLineEndIndex = -1;
	for (i = 0; str[i] != '\0'; i++) {
		if (str[i] == ' ') {
			lastSpaceIndex = i;
		}
		if (i - lastLineEndIndex - 1 == k || str[i] == '\n') {
			char* line = format(k, str, lastLineEndIndex + 1, 
				str[i] == '\n' ? i - 1 : lastSpaceIndex - 1);
			printf("%s\n", line);
			free(line);
			lastLineEndIndex = lastSpaceIndex;
		}
	}
	return 0;
}
