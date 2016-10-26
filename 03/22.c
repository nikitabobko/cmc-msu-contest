#include <stdio.h>
#include <stdlib.h>

void addOneToDigit(char** arr, int digitIndex, int* countOfNumbers) {
	if (digitIndex == -1) {
		int i;
		for (i = *countOfNumbers; i >= 1; i--) {
			(*arr)[i] = (*arr)[i-1];
		}
		(*arr)[0] = 0;
		digitIndex = 0;
		(*countOfNumbers)++;
	}
	(*arr)[digitIndex]++;
	if ((*arr)[digitIndex] == 10) {
		(*arr)[digitIndex] = 0;
		addOneToDigit(arr, digitIndex-1, countOfNumbers);
	}
}

void transform(char** string, int* countOfNumbers) {
	int i, sum1 = 0, sum2 = 0;
	for (i = 0; i < (*countOfNumbers) / 2; i++) {
		sum1 += (*string)[i];
	}
	for (i = (*countOfNumbers) / 2; i < (*countOfNumbers); i++) {
		sum2 += (*string)[i];
	}
	if (sum1 > sum2) {
		int offset = sum1 - sum2;
		for (i = (*countOfNumbers) - 1; i >= (*countOfNumbers) / 2; i--) {
			if (offset + (*string)[i] <= 9) {
				(*string)[i] += offset;
				break;
			} else {
				offset -= (9 - (*string)[i]);
				(*string)[i] = 9;
			}
		}
	} else if (sum1 < sum2) {
		int sum = 0;
		for (i = (*countOfNumbers) / 2; i < (*countOfNumbers); i++) {
			sum += (*string)[i];
			if (sum >= sum1) {
				//(*string)[i-1]++;
				addOneToDigit(string, i-1, countOfNumbers);
				int j;
				for (j = i; j < (*countOfNumbers); j++) {
					(*string)[j] = 0;
				}
				transform(string, countOfNumbers);
				break;
			} 
		}
	}
}

int main(void) {
	char string[100];
	scanf("%s", string);
	int i;
	for (i = 0; string[i] != '\0'; i++) {
		string[i] -= '0';
	}
	int countOfNumbers = i;
	// Exclusive situation when were entered numbers from 1 to 9
	if (countOfNumbers == 1) {
		printf("11");
		return 0;
	}
	char* temp = ((char *)string);
	transform(&temp, &countOfNumbers);
	for (i = 0; i < countOfNumbers; i++) {
		printf("%d", string[i]);
	}
	return 0;
}
