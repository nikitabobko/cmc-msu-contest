#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Prefix function
int prefixFun(char* const s1, char* const s2) {
	int len1 = strlen(s1), len2 = strlen(s2), len = len1 + len2 + 1;
	char* s = malloc(len1 + len2 + 1);
	for (int i = 0; i < len; i++) {
		if (i < len1) s[i] = s1[i];
		else if (i == len1) {
			s[i] = '#';
		} else s[i] = s2[i - len1 - 1];
	}
	int* p = malloc(len * sizeof(int));
	p[0] = 0;
	int k = p[0];
	for (int i = 1; i < len; i++) {
		for (;s[k] != s[i] && k > 0; k = p[k - 1]);
		if (s[k] == s[i]) {
			k++;
		}
		p[i] = k;
	}
	return p[len - 1];
}

int main(void) {
	char str1[1000001];
	char str2[1000001];
	scanf("%s%s", str1, str2);
	printf("%d %d", prefixFun(str1, str2), prefixFun(str2, str1));
	return 0;
}
