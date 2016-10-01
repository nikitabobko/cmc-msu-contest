#include <stdio.h>

double absx(double a) {
	return a < 0 ? (-1)*a : a;
}

double min(double a, double b) {
	return a < b ? a : b;
}

int main(void) {
	double a1, a2, b1, b2, c1, c2;
	scanf("%lf %lf %lf %lf %lf %lf", &a1, &a2, &b1, &b2, &c1, &c2);
	double d1 = absx(b1-c1)/2 + min(b1, c1);
	double d2 = absx(b2-c2)/2 + min(b2, c2);
	printf("%lf %lf", absx(d1-a1)*(a1 < d1 ? 2 : 1)/3 + min(a1, d1), 
		absx(d2-a2)*(a2 < d2 ? 2 : 1)/3 + min(a2, d2));
	return 0;
}