#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Returs index2 when str starts with index2 is smaller
// Returs index1 when str starts with index1 is smaller
int whichSmaller(char* str, int index1, int index2, int strLen) {
	int i;
	for (i = 0 ; i < strLen; i++) {
		if (str[(index1 + i) % strLen] > str[(index2 + i) % strLen]) {
			return index2; 
		} else if (str[(index1 + i) % strLen] < str[(index2 + i) % strLen]) {
			return index1;
		}
	}
	return index1;
}

int main(void) {
	char str[2000001];
	scanf("%s", str);
	int* indexes = malloc(2000000 * sizeof(int));
	indexes[0] = 0;
	int i, maxNumOfConsecutiveZeros = 0, curNumOfConsecutiveZeros = 0, indexesLen = 1;
	int strLen = strlen(str);
	for (i = 0; str[i] != '\n' && str[i] != '\0';) {
		if (str[i] == '0') {
			int curIndex = i;
			curNumOfConsecutiveZeros = 1;
			for (i++; str[i % strLen] == '0' && i % strLen != curIndex; i++) {
				curNumOfConsecutiveZeros++;
			}
			if (curNumOfConsecutiveZeros > maxNumOfConsecutiveZeros) {
				indexesLen = 1;
				indexes[0] = curIndex;
				maxNumOfConsecutiveZeros = curNumOfConsecutiveZeros;
			} else if (curNumOfConsecutiveZeros == maxNumOfConsecutiveZeros) {
				indexes[indexesLen] = curIndex;
				indexesLen++;
			}
		} else i++;
	}
	for (i = 1; i < indexesLen; i++) {
		indexes[i] = whichSmaller(str, indexes[i], indexes[i-1], strLen);
	}
	for (i = 0; i < strLen; i++) {
		printf("%c", str[(indexes[indexesLen - 1] + i) % strLen]);
	}
	return 0;
}
