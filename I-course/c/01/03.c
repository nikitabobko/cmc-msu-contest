#include <stdio.h>

int main(void) {
    int n;
    scanf("%d", &n);
    while (1 == 1) {
	char c;
	scanf("%c", &c);
	if (c == ' ') {
	    printf(" ");
	    continue;
	}
	if (c == '.') {
	    printf(".");
	    return 0;
	}
	char z, a;
	if (c >= 'a' && c <= 'z') {
	    a = 'a';
	    z = 'z';
	} else {
	    a = 'A';
	    z = 'Z';
	}
	int sym = c + n;
	printf("%c", (char)(((sym-a) % (z-a+1)) + a));
    }
    return 0;
}