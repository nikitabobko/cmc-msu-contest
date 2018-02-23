#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "system_of_linear_equations_solver.h"

static const char *FORMULA_FLAG = "--formula";
static const char *FORMULA_FLAG_PROTOTYPE = "--formula <n> <m> <variant>";
static const char *FORMULA_FLAG_DESCRIPTION =
        "use special formula for computing A and f matrices elements (Ax = f). "
        "If use this flag specifying formula parameters (<n>, <m> and <variant>) are "
        "mandatory. Flag is valid only in mode_dfl.";

static const char *OPTIMIZE_FLAG = "--optimize";
static const char *OPTIMIZE_FLAG_DESCRIPTION =
        "whether to optimize matrix pivot element choice in Gaussian "
        "elimination method while getting echelon form.";

static const char *INPUT_FILE_FLAG = "--file";
static const char *INPUT_FILE_FLAG_DESCRIPTION =
        "specify the file to read matrix from. If not specified stdin is used "
        "by default.";


static const char *DIMENSION_FLAG = "--dimension";
static const char *DIMENSION_FLAG_PROTOTYPE = "--dimension <n>";
static const char *DIMENSION_FLAG_DESCRIPTION = "mandatory flag. Use this to specify exact matrix dimension.";

static const char *MODE_FLAG = "--mode";
static const char *MODE_FLAG_PROTOTYPE = "--mode <mode>";
static const char *MODE_FLAG_DESCRIPTION =
        "specify mode for program execution. <mode> can have one of these values: "
        "mode_dfl (default mode for solving system of linear equations), mode_det "
        "(compute matrix determinant), mode_inv (compute invertible matrix)";

static const char *MODE_DFL_STR = "mode_dfl";
static const char *MODE_INVERTIBLE_STR = "mode_inv";
static const char *MODE_DET_STR = "mode_det";

typedef enum Mode {
    MODE_DFL, MODE_INVERTIBLE, MODE_DET
} Mode;

typedef enum InputType {
    Matrix, LinearEquationsSystem, InvertibleMatrixBlank
} InputType;

void print_help(const char *exe) {
    const char *h[][2] = {
            {"-h --help", "show this help"},
            {INPUT_FILE_FLAG, INPUT_FILE_FLAG_DESCRIPTION},
            {FORMULA_FLAG_PROTOTYPE, FORMULA_FLAG_DESCRIPTION},
            {OPTIMIZE_FLAG, OPTIMIZE_FLAG_DESCRIPTION},
            {DIMENSION_FLAG_PROTOTYPE, DIMENSION_FLAG_DESCRIPTION},
            {MODE_FLAG_PROTOTYPE, MODE_FLAG_DESCRIPTION},
    };
    printf("Usage: %s [-options]\n"
           "where [-options] includes:\n", exe);
    for (int i = 0; i < sizeof(h)/sizeof(*h); i++) {
        printf("   %-29s %s\n", h[i][0], h[i][1]);
    }
}


void parse_arguments(const char **argv, int argv_len, int *formula, int *n, int *m, int *variant, int *optimize,
        FILE **file, int *dim, Mode *mode) {
    if (mode) *mode = MODE_DFL;
    if (formula) *formula = 0;
    if (optimize) *optimize = 0;
    if (file) *file = stdin;
    for (int i = 1; i < argv_len; i++) {
        if (optimize && !strcmp(argv[i], OPTIMIZE_FLAG)) {
            *optimize = 1;
        } else if (file && !strcmp(argv[i], INPUT_FILE_FLAG)) {
            if (i > argv_len - 1 - 1) {
                print_help(argv[0]);
                exit(1);
            }
            errno = 0;
            FILE *f = fopen(argv[++i], "r");
            if (!f) {
                strerror(errno);
                exit(errno);
            }
        } else if (formula && m && n && variant && !strcmp(argv[i], FORMULA_FLAG)) {
            *formula = 1;
            if (i > argv_len - 1 - 3) {
                print_help(argv[0]);
                exit(1);
            }
            errno = 0;
            char *c;
            *n = strtol(argv[++i], &c, 10);
            if (errno || *c != '\0') {
                print_help(argv[0]);
                exit(1);
            }
            *m = strtol(argv[++i], &c, 10);
            if (errno || *c != '\0') {
                print_help(argv[0]);
                exit(1);
            }
            *variant = strtol(argv[++i], &c, 10);
            if (errno || *c != '\0') {
                print_help(argv[0]);
                exit(1);
            }
        } else if (dim && !strcmp(argv[i], DIMENSION_FLAG)) {
            if (i > argv_len - 1 - 1) {
                print_help(argv[0]);
                exit(1);
            }
            char *c;
            *dim = strtol(argv[++i], &c, 10);
            if (errno || *c != '\0') {
                print_help(argv[0]);
                exit(1);
            }
        } else if (mode && !strcmp(argv[i], MODE_FLAG)) {
            if (i > argv_len - 1 - 1) {
                print_help(argv[0]);
                exit(1);
            }
            i++;
            if (!strcmp(argv[i], MODE_DFL_STR)) {
                *mode = MODE_DFL;
            } else if (!strcmp(argv[i], MODE_DET_STR)) {
                *mode = MODE_DET;
            } else if (!strcmp(argv[i], MODE_INVERTIBLE_STR)) {
                *mode = MODE_INVERTIBLE;
            } else {
                print_help(argv[0]);
                exit(1);
            }
        } else {
            print_help(argv[0]);
            exit(0);
        }
    }
}

mat_elem **input(FILE *f, int dim, InputType type) {
    if (f == stdin) {
        printf("Enter matrix:\n");
    }

    mat_elem **matrix = malloc(dim * sizeof(*matrix));
    if (matrix == NULL) {
        return NULL;
    }
    for (int i = 0; i < dim; i++) {
        switch (type) {
            case LinearEquationsSystem:
                matrix[i] = malloc((dim + 1) * sizeof(**matrix));
                break;
            case Matrix:
                matrix[i] = malloc(dim * sizeof(**matrix));
                break;
            case InvertibleMatrixBlank:
                matrix[i] = malloc(2 * dim * sizeof(**matrix));
                break;
        }
        if (matrix[i] == NULL) {
            perror(strerror(errno));
            exit(errno);
        }
        int n = 0;
        switch (type) {
            case LinearEquationsSystem:
                n = dim + 1;
                break;
            case Matrix:
                n = dim;
                break;
            case InvertibleMatrixBlank:
                n = dim;
                break;
        }
        for (int j = 0; j < n; j++) {
            fscanf(f, "%Lf", &(matrix[i][j]));
        }
    }
    return matrix;
}

void print_system_of_linear_equations_solution(mat_elem *solution, int dim) {
    if (solution == NULL) {
        return;
    }
    for (int i = 0; i < dim; i++) {
        printf("x_%d = %Lf\n", i + 1, solution[i]);
    }
}

void fill_matrix_with_formula(mat_elem **matrix, int dim, int n, int m, int variant) {
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            if (i == j) {
                matrix[i][j] = n + m*m + j/m + i/n;
            } else {
                matrix[i][j] = (i + j)/(m + n);
            }
        }
    }
    for (int i = 0; i < dim; i++) {
        switch (variant) {
            case 1:
                matrix[i][dim] = n * i + m;
                break;
            case 2:
                matrix[i][dim] = 200 + 50 * i;
                break;
            case 3:
                matrix[i][dim] = i*i - 100;
                break;
            case 4:
                matrix[i][dim] = m*n - i*i*i;
                break;
            case 5:
                matrix[i][dim] = m*i + n;
                break;
            default:
                fprintf(stderr, "Only variants from 1 to 5 are supported");
                exit(1);
        }
    }
}

void mode_dfl(int use_formula, FILE *file, int dim, int optimize, int n, int m, int variant) {
    printf("System of linear equations solver mode\n");
    mat_elem **matrix;
    if (!use_formula) {
        matrix = input(file, dim, LinearEquationsSystem);
        if (!matrix) {
            fprintf(stderr, "Error while inputing matrix.\n");
            exit(1);
        }
    } else {
        matrix = malloc(dim * sizeof(*matrix));
        if (!matrix) {
            perror(strerror(errno));
            exit(errno);
        }
        for (int i = 0; i < dim; i++) {
            matrix[i] = malloc((dim + 1)* sizeof(**matrix));
            if (!matrix[i]) {
                perror(strerror(errno));
                exit(errno);
            }
        }
        fill_matrix_with_formula(matrix, dim, n, m, variant);
    }
    mat_elem *solution = solve_system_of_linear_equations(matrix, dim, optimize);
    if (solution == NULL) {
        fprintf(stderr, "Error occurred while solving system of linear equations");
    } else {
        print_system_of_linear_equations_solution(solution, dim);
    }
    for (int i = 0; i < dim; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

void mode_det(FILE *file, int dim) {
    printf("Compute determinant mode\n");
    mat_elem **matrix = input(file, dim, Matrix);
    if (!matrix) {
        fprintf(stderr, "Error while inputing matrix.\n");
        exit(1);
    }

    printf("Determinant: %Lf\n", determinant(matrix, dim));
}

void mode_invertible(FILE *file, int dim) {
    printf("Compute invertible matrix mode\n");
    mat_elem **matrix = input(file, dim, InvertibleMatrixBlank);
    if (!matrix) {
        fprintf(stderr, "Error while inputing matrix.\n");
        exit(1);
    }
    printf("Invertible matrix:\n");
    mat_elem **result = get_invertible_matrix(matrix, dim);

    print_matrix(result, dim, dim);

    free_matrix(matrix, dim);
    free_matrix(result, dim);
}

int main(int argc, char const **argv) {
    int use_formula, optimize, m, n, variant, dim = -1;
    Mode mode;
    FILE *file;
    parse_arguments(argv, argc, &use_formula, &n, &m, &variant, &optimize, &file, &dim, &mode);
    if (dim < 0) {
        fprintf(stderr, "\"%s\" flag is mandatory.\n", DIMENSION_FLAG);
        exit(1);
    }

    switch (mode) {
        case MODE_DFL:
            mode_dfl(use_formula, file, dim, optimize, n, m, variant);
            break;
        case MODE_DET:
            mode_det(file, dim);
            break;
        case MODE_INVERTIBLE:
            mode_invertible(file, dim);
            break;
    }

    return 0;
}