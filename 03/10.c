#include <stdio.h>
#include <assert.h>

long long int recursive(void) {
	long long int number = 0;
	char c = 0, isNumber = 1, isPositiveNumber = 1;
	while (1) {
		char temp;
		if (scanf("%c", &temp) == EOF) break;
		if (temp == ' ' || temp == '\n') break;
		c = temp;
		if (c >= '0' && c <= '9') {
			number = number * 10 + (c - '0');
		} else if (c == '-') {
			isPositiveNumber = 0;
		} else {
			isNumber = 0;
		}
	}
	if (isNumber) return number * (isPositiveNumber ? 1 : (-1));
	else {
		switch(c) {
			case '*':
				return recursive() * recursive();
			case '/':
				return recursive() / recursive();
			default:
				assert(0);
		}
	}
}

int main(void) {
	printf("%lld\n", recursive());
	return 0;
}
