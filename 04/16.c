#include <stdio.h>
#include <string.h>

// Returs 1 when str starts with index2 is smaller
// Returs -1 when str starts with index1 is smaller
// Returs 0...
int whichSmaller(char* str, int index1, int index2, int strLen) {
	int i;
	for (i = 0 ; i < strLen; i++) {
		if (str[(index1 + i) % strLen] > str[(index2 + i) % strLen]) {
			return 1; 
		} else if (str[(index1 + i) % strLen] < str[(index2 + i) % strLen]) {
			return -1;
		}
	}
	return 0;
}

int main(void) {
	char str[2000001];
	scanf("%s", str);
	int i, maxNumOfConsecutiveZeros = 0, curNumOfConsecutiveZeros = 0, index = 0;
	int strLen = strlen(str);
	for (i = 0; str[i] != '\n' && str[i] != '\0';) {
		if (str[i] == '0') {
			int curIndex = i;
			curNumOfConsecutiveZeros = 1;
			for (i++; str[i % strLen] == '0' && i % strLen != curIndex; i++) {
				curNumOfConsecutiveZeros++;
			}
			if (curNumOfConsecutiveZeros > maxNumOfConsecutiveZeros) {
				index = curIndex;
				maxNumOfConsecutiveZeros = curNumOfConsecutiveZeros;
			} else if (curNumOfConsecutiveZeros == maxNumOfConsecutiveZeros) {
				if (whichSmaller(str, curIndex, index, strLen) == -1) {
					index = curIndex;
				}
			}
		} else i++;
	}
	for (i = 0; i < strLen; i++) {
		printf("%c", str[(index + i) % strLen]);
	}
	return 0;
}
