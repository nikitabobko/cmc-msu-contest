#include <stdio.h>

int main(void) {
	int i, result;
	scanf("%d", &result);
	if (result == 0) {
		printf("0");
		return 0;
	}
	for (i = 0; i >= 0; i++) {
		int var1, var2;
		scanf("%d", &var1);
		if (var1 == 0) break;
		scanf("%d", &var2);
		if (var2 == 0) {
			result += var1;
			break;
		}
		result += var1*var2;
	}
	printf("%d", result);
	return 0;
}
