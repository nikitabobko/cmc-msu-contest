#include <stdio.h>

int main(void) {
	unsigned int n, summ = 0, powOfThree = 1;
	scanf("%d", &n); 
	int count = 0;
	while(n > 0) {
		int ost = n % 3;
		if (ost == 2) count++, n++;
		else if (ost == 1) {
			count++;
			summ += powOfThree;
		}
		n /= 3, powOfThree *= 3;
	}
	if (summ > 1000000) printf("-1\n");
	else printf("%d\n", count); 
	return 0;
}