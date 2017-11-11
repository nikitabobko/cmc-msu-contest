#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <limits.h>
#include <errno.h>

enum 
{
    NUMERICAL_SYSTEM_BASE = 10
};

#define MIN(a,b) (((a)<(b))?(a):(b))

double from_little_endian(unsigned char *mem) {
    double num;
    unsigned long long *buf = (unsigned long long *) &num;
    *buf = 0;
    for (int i = sizeof(num) - 1; i >= 0; i++) {
        (*buf) <<= CHAR_BIT;
        (*buf) |= mem[i];
    }
    return num;
}

void to_little_endian(unsigned char *mem, double num) {
    unsigned long long *buf = (unsigned long long *) &num;
    for (int i = 0; i < sizeof(num); i++) {
        mem[i] = *buf;
        (*buf) >>= CHAR_BIT;
    }
}

double get_elem(unsigned char *mem, int i, int j, int dim) {
    if (i == j) {
        return 0.;
    }
    if (i > j) {
        int t = i;
        i = j;
        j = t;
    }
    return from_little_endian(mem + (i * dim + j) * sizeof(double));
}

void fill_with_identity_matrix(char *mem, int dim) {
    for (int i = 0; i < dim - 1; i++) {
        for (int j = i + 1; j < dim; j++) {
            to_little_endian(mem + (i * dim + j) * sizeof(double), INFINITY);
        }
    }
}

void pow_matrix(unsigned char *mem, int dim, int power) {
    if (power == 0) {
        fill_with_identity_matrix(mem, dim);
    }
    for (int p = 1; p < power; p++) {
     
        for (int i = 0; i < dim - 1; i++) {
            for (int j = i + 1; j < dim; j++) {

                for (int k = 0; k < dim; k++) {
                    double d = get_elem(mem, i, k, dim) + get_elem(mem, k, j, dim);
                    double cur = get_elem(mem, i, j, dim);

                    to_little_endian(mem + (i * dim + j) * sizeof(double), 
                            k == 0 ? d : MIN(cur, d));
                }

            }
        }

    }

    for (int i = 1; i < dim; i++) {
        for (int j = 0; j < i; j++) {
            memcpy(mem + (j * dim + i) * sizeof(double), mem + (i * dim + j) * sizeof(double), 
                    sizeof(double));
        }
    }
}

void fill(char *mem, int dim) {
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            double num;
            scanf("%lf", &num);
            num = (num == -1. ? INFINITY : num);
            to_little_endian(mem + (i * dim + j) * sizeof(double), num);
        }    
    }
}

int main(int argc, char const *argv[]) {
    if (argc != 3) {
        return 1;
    }

    int out_fd = open(argv[1], O_RDWR | O_CREAT | O_TRUNC, 0666);
    if (out_fd < 0) {
        return 1;
    }

    int dim = strtol(argv[2], NULL, 10);
    int size = dim * dim * sizeof(double);

    if (lseek(out_fd, size - 1, SEEK_SET) < 0) {
        printf("GOVNO\n");
    }
    char terminator = '\0';
    write(out_fd, &terminator, sizeof(terminator));
    
    unsigned char *out_mem = mmap(NULL, size, PROT_WRITE, MAP_SHARED, out_fd, 0);

    fill(out_mem, dim);

    close(out_fd);
    return 0;
}