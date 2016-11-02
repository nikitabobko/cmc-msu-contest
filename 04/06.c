#include <stdio.h>

int main(void) {
	int k;
	scanf("%d", &k);
	char str[k + 2];
	getchar();
	fgets(str, k + 2, stdin);
	int i, numberOfSpaces = 0;
	int spacesArr[k];
	for (i = 0; str[i] != '\n'; i++) {
		spacesArr[numberOfSpaces] = 0;
		if (str[i] == ' ') {
			while (str[i] == ' ') {
				spacesArr[numberOfSpaces]++;
				i++;
			}
			numberOfSpaces++;
			i--;
		}
	}
	// i - lenght
	for (i = k - i; i > 0; i--) {
		int minIndex = 0, j;
		for (j = 1; j < numberOfSpaces; j++) {
			if (spacesArr[j] < spacesArr[minIndex] || 
					(spacesArr[j] == spacesArr[minIndex] && j < minIndex)) {
				minIndex = j;
			}
		}
		spacesArr[minIndex]++;
	}
	// Now use numberOfSpaces as index
	numberOfSpaces = 0;
	for (i = 0; str[i] != '\0'; i++) {
		if (str[i] != ' ') printf("%c", str[i]);
		else {
			while (str[i] == ' ') {
				i++;
			}
			i--;
			int j;
			for (j = 0; j < spacesArr[numberOfSpaces]; j++) {
				printf(" ");
			}
			numberOfSpaces++;
		} 
	}
	return 0;
}
