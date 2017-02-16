#include <stdio.h>

int isPrime(int n) {
	if (n == 1) return 0;
	if (n % 2 == 0) return (n == 2);
	for (int i = 3; i*i <= n; i++) {
		if (n % i == 0) return 0;
	}
	return 1;
}

void function(int n, int curNumbersNumber, int requeredNumbersNumber) {
	if (curNumbersNumber == requeredNumbersNumber) {
		if (isPrime(n)) printf("%d ", n);
		return;
	}
	if (isPrime(n)) {
		curNumbersNumber++;
		function(n * 10 + 1, curNumbersNumber, requeredNumbersNumber);
		function(n * 10 + 3, curNumbersNumber, requeredNumbersNumber);
		function(n * 10 + 7, curNumbersNumber, requeredNumbersNumber);
		function(n * 10 + 9, curNumbersNumber, requeredNumbersNumber);
	}
}

int main(void) {
	int n;
	scanf("%d", &n);
	function(2, 1, n);
	function(3, 1, n);
	function(5, 1, n);
	function(7, 1, n);
	return 0;
}
