#include <stdio.h>

int main(void) {
	int matrix[8][8];
	int i, count = 64;
	for (i = 0; i < 64; i++) matrix[i / 8][i % 8] = 0;
	while(1) {
		char y;
		scanf("%c", &y);
		if (y == '\n') break;
		y -= 'a';
		int x;
		scanf("%d", &x);
		x--;
		if (!matrix[(unsigned char)y][x]) count--;
		matrix[(unsigned char)y][x] = 1;
		int j;
		for (j = 0; j < 9; j++) {
			int y1 = j / 3 - 1 + y, x1 = j % 3 - 1 + x;
			if (y1 >= 0 && y1 <= 7 && x1 >= 0 && x1 <= 7 && 
					!matrix[y1][x1]) { 
				count--;
				matrix[y1][x1] = 1; 
			}
		}
	}
	printf("%d\n", count);
	return 0;
}