#include <stdio.h>

int main(void) {
	int n, numberOfZeros = 0;
	scanf("%d", &n);
	long long int positive1 = 0, positive2 = 0, negative1 = 0, negative2 = 0;
	short isPositive1Defined = 0, isPositive2Defined = 0,
		isNegative1Defined = 0, isNegative2Defined = 0;
	int i;
	for (i = 0; i < n; i++) {
		long long int var;
		scanf("%lld", &var);
		if (var == 0) numberOfZeros++;
		else if (var > 0) {
			if (!isPositive1Defined || var > positive1) {
				positive2 = positive1;
				positive1 = var;
				isPositive2Defined = isPositive1Defined;
				isPositive1Defined = 1;
			} else if (!isPositive2Defined || var > positive2) {
				positive2 = var;
				isPositive2Defined = 1;
			}
		} else {	// var < 0
			if (!isNegative1Defined || var < negative1) {
				negative2 = negative1;
				negative1 = var;
				isNegative2Defined = isNegative1Defined;
				isNegative1Defined = 1;
			} else if (!isNegative2Defined || var < negative2) {
				negative2 = var;
				isNegative2Defined = 1;
			}
		}
	}
	if (numberOfZeros == n) printf("0 0");
	else if (numberOfZeros == n-1) {
		if (isNegative1Defined) printf("%lld 0", negative1);
		else if (numberOfZeros >= 2) printf("0 0");
		else printf("0 %lld", positive1);
	} else {
		if (isPositive1Defined && isPositive2Defined && isNegative1Defined && isNegative2Defined) {
			if (negative1 * negative2 >= positive1 * positive2) printf("%lld %lld", negative1, negative2);
			else printf("%lld %lld", positive2, positive1);
		} else if (isPositive1Defined && isPositive2Defined) printf("%lld %lld", positive2, positive1);
		else if (isNegative1Defined && isNegative2Defined) printf("%lld %lld", negative1, negative2);
		else if (numberOfZeros >= 1) printf("%lld 0", negative1);
		else printf("%lld %lld", negative1, positive1);
	}
	return 0;
}
