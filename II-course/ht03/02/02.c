/*
Submit a solution for ht03-2-unix/mmap/mmap-2

Full score: 100
Run penalty:    10
Time limit: 2 s
Real time limit:    5 s
Memory limit:   64M
Open date:  2017/10/31 10:30:00
Date penalty:   2017/12/31 0-50/14d
Problem ht03-2: unix/mmap/mmap-2

Программе задаются три аргумента командной строки.

Путь IN к входному файлу.
Путь OUT к выходному файлу.
Число P неотрицательное число, представимое типом int.
В бинарном файле IN находится квадратная матрица вещественных чисел типа double. Данные хранятся в представлении LE. На диагонали матрицы записаны 0, матрица симметричная и все ее элементы неотрицательны. Файл не пустой. Размер размер матрицы не превосходит 128x128.

В выходной бинарный файл OUT запишите квадратную матрицу того же размера и того же типа, полученную в результате вычислений, описанных ниже.

Специальным произведением двух квадратных матриц A и B одинакового размера назовем произведение, вычисляемое по стандартным правилам, но в котором вместо операции сложения используется операция взятия минимума, а вместо операции умножения — операция сложения. Таким образом,
a[i][j] = min (a[i][k] + a[k][j])

Единичной матрицей для таким образом определенной операцией умножения является матрица, на главной диагонали которой находятся 0, а остальные элементы равны положительной бесконечности.

В выходной файл OUT запишите входную матрицу IN возвреденную в степень P с помощью операции специального произведения.

Для обработки файла и хранения временных данных используйте файлы, отображаемые в память (mmap).

Все данные корректны и удовлетворяют ограничениям.
*/
#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <limits.h>
#include <errno.h>
#include <sys/stat.h>

enum
{
    NUMERICAL_SYSTEM_BASE = 10,
};

#define MIN(a,b) (((a)<(b))?(a):(b))

static inline double get_elem(double *mem, int i, int j, int dim) {
    if (i == j) {
        return 0.;
    }
    if (i > j) {
        int t = i;
        i = j;
        j = t;
    }
    return mem[i * dim + j];
}

void fill_with_identity_matrix(double *mem, int dim) {
    for (int i = 0; i < dim - 1; i++) {
        for (int j = i + 1; j < dim; j++) {
            mem[i * dim + j] = INFINITY;
        }
    }
}

void multiply(double *res, double *mem1, double *mem2,
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
            res[i * dim + j] = cur;
            if (mem1_cur > cur || mem1_cur < cur) {
                changes_happen_local = 1;
            }
        }
    }
    if (changes_happen != NULL) {
        *changes_happen = changes_happen_local;
    }
}

double *pow_matrix(double *in_mem, int dim, int power, int size) {
    double *mem = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS,
            -1, 0);
    double *res = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS,
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
            double *t = res;
            res = mem;
            mem = t;
        }

        if (changes_happen2) {
            multiply(mem, in_mem, in_mem, dim, &changes_happen2);
            double *t = in_mem;
            in_mem = mem;
            mem = t;
        }

        power /= 2;
    }

    // Fill under diagonal part of matrix
    for (int i = 1; i < dim; i++) {
        for (int j = 0; j < i; j++) {
            memcpy(res + (i * dim + j), res + (j * dim + i), sizeof(double));
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

    struct stat info;
    if (fstat(in_fd, &info) < 0) {
        return 1;
    }
    off_t size = info.st_size;

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

    double *in_mem = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE, in_fd, 0);
    double *out_mem = mmap(NULL, size, PROT_WRITE, MAP_SHARED, out_fd, 0);
    if (in_mem == MAP_FAILED || out_mem == MAP_FAILED) {
        return 1;
    }

    double *mem = pow_matrix(in_mem, dim, power, size);
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