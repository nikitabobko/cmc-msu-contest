#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <limits.h>
#include <errno.h>

enum 
{
    NUMERICAL_SYSTEM_BASE = 10,
};

#define MIN(a,b) (((a)<(b))?(a):(b))

// Convert from little endian format to machine endian format
static inline double from_little_endian(unsigned char *mem) {
    double num = 0;
    unsigned long long *buf = (unsigned long long *) &num;
    *buf = 0;
    for (int i = sizeof(num) - 1; i >= 0; i--) {
        (*buf) <<= CHAR_BIT;
        (*buf) |= mem[i];
    }
    return num;
}

// Convert from machine endian format to little endian format
static inline void to_little_endian(unsigned char *mem, double num) {
    unsigned long long *buf = (unsigned long long *) &num;
    for (int i = 0; i < sizeof(num); i++) {
        mem[i] = *buf;
        (*buf) >>= CHAR_BIT;
    }
}

static inline double get_elem(unsigned char *mem, int i, int j, int dim) {
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

void fill_with_identity_matrix(unsigned char *mem, int dim) {
    for (int i = 0; i < dim - 1; i++) {
        for (int j = i + 1; j < dim; j++) {
            to_little_endian(mem + (i * dim + j) * sizeof(double), INFINITY);
        }
    }
}

void multiply(unsigned char *res, unsigned char *mem1, unsigned char *mem2, 
        int dim, int *changes_happen) {
    int changes_happen_local = 0;
    for (int i = 0; i < dim - 1; i++) {
        for (int j = i + 1; j < dim; j++) {
            double mem1_cur = 0;
            double cur = 0;
            for (int k = 0; k < dim; k++) {
                double temp = get_elem(mem1, i, k, dim);
                if (k == j) {
                    mem1_cur = temp;
                }
                double d = temp + get_elem(mem2, k, j, dim);
                
                cur = (k == 0 ? d : MIN(cur, d));

            }
            to_little_endian(res + (i * dim + j) * sizeof(double), cur);
            if (mem1_cur > cur || mem1_cur < cur) {
                changes_happen_local = 1;
            }
        }
    }
    if (changes_happen != NULL) {
        *changes_happen = changes_happen_local;
    }
}

unsigned char *pow_matrix(unsigned char *in_mem, int dim, int power, int size) {
    unsigned char *mem = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, 
            -1, 0);
    unsigned char *res = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, 
            -1, 0);
    if (mem == MAP_FAILED || res == MAP_FAILED) {
        munmap(in_mem, size);
        if (mem != MAP_FAILED) {
            munmap(mem, size);
        } 
        if (res != MAP_FAILED) {
            munmap(res, size);
        }
        return NULL;
    }
    fill_with_identity_matrix(res, dim);

    int changes_happen2 = 1;
    int changes_happen1 = 1;
    while (power > 0 && (changes_happen2 || changes_happen1)) {
        if (power & 0x1 && changes_happen1) {
            multiply(mem, res, in_mem, dim, &changes_happen1);
            unsigned char *t = res;
            res = mem;
            mem = t;
        }

        if (changes_happen2) {
            multiply(mem, in_mem, in_mem, dim, &changes_happen2);
            unsigned char *t = in_mem;
            in_mem = mem;
            mem = t;
        }

        power /= 2;
    }

    // Fill under diagonal part of matrix
    for (int i = 1; i < dim; i++) {
        for (int j = 0; j < i; j++) {
            memcpy(res + (i * dim + j) * sizeof(double), res + (j * dim + i) * sizeof(double), 
                    sizeof(double));
        }
    }
    munmap(in_mem, size);
    munmap(mem, size);
    return res;
}

int main(int argc, char const *argv[]) {
    if (argc != 4) {
        return 1;
    }

    int in_fd = open(argv[1], O_RDONLY);
    int out_fd = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, 0666);
    if (in_fd < 0 || out_fd < 0) {
        return 1;
    }
    int size = lseek(in_fd, 0, SEEK_END);
    if (size == -1) {
        return 1;
    }

    if (lseek(out_fd, size - 1, SEEK_SET) < 0) {
        return 1;
    }
    char terminator = '\0';
    if (write(out_fd, &terminator, sizeof(terminator)) <= 0) {
        return 1;
    }

    // dimension
    int dim = sqrt(size / sizeof(double));
    
    errno = 0;
    char *endptr;
    int power = strtol(argv[3], &endptr, NUMERICAL_SYSTEM_BASE);
    if (errno || *endptr != '\0' || !strcmp(argv[3], "") || power < 0) {
        return 0;
    }

    unsigned char *in_mem = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE, in_fd, 0);
    unsigned char *out_mem = mmap(NULL, size, PROT_WRITE, MAP_SHARED, out_fd, 0);
    if (in_mem == MAP_FAILED || out_mem == MAP_FAILED) {
        return 1;
    }

    unsigned char *mem = pow_matrix(in_mem, dim, power, size);
    if (mem == NULL) {
        munmap(out_mem, size);
    }

    memcpy(out_mem, mem, size);

    munmap(out_mem, size);
    munmap(mem, size);

    close(in_fd);
    close(out_fd);
    return 0;
}