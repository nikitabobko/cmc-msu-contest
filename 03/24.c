#include <stdio.h>

int lighten(int indexOfLastBulb, int isLastBulbLightned, int howManyCanLightBefore, 
		int n, int howManyWantToLight) {
	if (n == 0) return 0;
	long long int number;
	scanf("%lld", &number);
	int addition = 0;
	if (howManyWantToLight > 0) {
		addition++;
	}
	if (number < 0) {
		addition += ((howManyCanLightBefore + number) >= 0 ? -number : howManyCanLightBefore) 
			+ ((indexOfLastBulb != -1 && !isLastBulbLightned && indexOfLastBulb - n + number <= 0) ? 1 : 0);
	}
	if (number != 0) {
		indexOfLastBulb = n;
		isLastBulbLightned = howManyWantToLight > 0;
		howManyCanLightBefore = 0;
		howManyWantToLight = number > 0 ? number : 0;
	} else {
		if (howManyWantToLight > 0) howManyWantToLight--; 
		else howManyCanLightBefore++;
	}
	return addition + lighten(indexOfLastBulb, isLastBulbLightned, howManyCanLightBefore, 
		n - 1, howManyWantToLight);
}

int main(void) {
	int n;
	scanf("%d", &n);
	printf("%d\n", lighten(-1, 0, 0, n, 0));
	return 0;
}
