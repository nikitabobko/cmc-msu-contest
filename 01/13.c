#include <stdio.h>

int main(void) {
	//sin(x) = x - x3/3! + x5/5! - x7/7! + ... + x(2n+1)/(2n+1)!
	double x, result, powX;
	unsigned long long int bottom = 1;
	int n;
	scanf("%lf%d", &x, &n);
	powX = x;
	int i;
	for (i = 0; i < n; i++) {
		double val = powX/bottom;
		powX *= x*x;
		bottom *= (bottom+1)*(bottom+2);
		if (i % 2) result -= val;
		else result += val;
	}
	printf("%.6lf", result);
	return 0;
}
