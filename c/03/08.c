#include <stdio.h>

int getMedian(int a, int b, int c) {
	if ((a <= b && a >= c) || (a <= c && a >= b)) return a;
	else if ((b <= c && b >= a) || (b <= a && b >= c)) return b;
	else return c;
}

int main(void){
	int n;
	scanf("%d", &n);
	int arr[n];
	int i;
	for (i = 0; i < n; i++) {
		scanf("%d", arr + i);
	}
	while(n != 1) {
		for (i = 0; i < n - 2; i++) {
			arr[i] = getMedian(arr[i], arr[i+1], arr[i+2]);
		}
		n -= 2;
	}
	printf("%d\n", arr[0]);
	return 0;
}