#include <stdio.h>
#define MAX(X, Y) (((X) < (Y)) ? (Y) : (X))

int 
main(int argc, char const *argv[])
{
    if (argc < 4) {
        return 1;
    }
    double balance, deposit, credit;
    if (sscanf(argv[1], "%lf", &balance) != 1 || sscanf(argv[2], "%lf", &deposit) != 1 || 
        sscanf(argv[3], "%lf", &credit) != 1) {
        return 1;
    }
    for (int i = 0; i < MAX(1, argc - 4); i++) {
        double transaction = 0.0;
        if (i < argc - 4 && sscanf(argv[i+4], "%lf", &transaction) != 1) {
            return 1;
        }
        balance += transaction;
        balance = balance * (balance > 0 ? deposit : credit) / 100 + balance;
    }
    printf("%.10g\n", balance);
    return 0;
}