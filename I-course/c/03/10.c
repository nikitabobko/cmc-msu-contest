#include <stdio.h>
#include <assert.h>

int recursive(void) {
	int number = 0;
	char c, isNumber = 1, isPositiveNumber = 1;
	// Skip for spaces
	do {
		c = getchar();
	} while(c == ' ');
	char temp = c;
	while (temp != ' ' && temp != EOF && temp != '\n') {
		c = temp;
		if (c >= '0' && c <= '9') {
			number = number * 10 + (c - '0');
		} else if (c == '-') {
			isPositiveNumber = 0;
		} else {
			isNumber = 0;
		}
		temp = getchar();
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
	printf("%d\n", recursive());
	return 0;
}
