#include <stdio.h>
#include <stdlib.h>

int addOneToDigit(char** arr, int digitIndex) {
	(*arr)[digitIndex]++;
	if ((*arr)[digitIndex] == 10) {
		(*arr)[digitIndex] = 0;
		addOneToDigit(arr, digitIndex-1); 
		return 1;
	}
	return 0;
}

void transform(int sum1, int sum2, int reSumSum1AndSum2, char** string, int countOfNumbers) {
	int i;
	if (reSumSum1AndSum2) {
		sum1 = 0;
		sum2 = 0;
		for (i = 0; i < countOfNumbers / 2; i++) {
			sum1 += (*string)[i];
		}
		for (i = countOfNumbers / 2; i < countOfNumbers; i++) {
			sum2 += (*string)[i];
		}
	}
	if (sum1 > sum2) {
		int offset = sum1 - sum2;
		for (i = countOfNumbers - 1; i >= countOfNumbers / 2; i--) {
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
		for (i = countOfNumbers / 2; i < countOfNumbers; i++) {
			sum += (*string)[i];
			if (sum >= sum1) {
				//(*string)[i-1]++;
				int reSum = addOneToDigit(string, i-1);
				if (i != countOfNumbers / 2) {
					sum2++;	 	
				} else {
					sum1++;
				}
				int j;
				for (j = i; j < countOfNumbers; j++) {
					sum2 -= (*string)[j];
					(*string)[j] = 0;
				}
				transform(sum1, sum2, reSum, string, countOfNumbers);
				break;
			} 
		}
	}
}

int main(void) {
	char* string = malloc(81); 
	scanf("%s", string);
	int i;
	for (i = 0; string[i] != '\0'; i++) {
		string[i] -= '0';
	}
	int countOfNumbers = i;
	// Exclusive sutiation when were entered numbers from 1 to 9
	if (countOfNumbers == 1) {
		printf("11");
		return 0;
	}
	transform(0, 0, 1, &string, countOfNumbers);
	for (i = 0; i < countOfNumbers; i++) {
		printf("%d", string[i]);
	}
	return 0;
}
