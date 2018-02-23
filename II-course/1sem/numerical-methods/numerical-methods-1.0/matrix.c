#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"

static void swap_matrix_columns(mat_elem **matrix, int columnOneIndex, int columnTwoIndex,
                                int num_of_lines, int startFrom) {
    if (columnOneIndex == columnTwoIndex) {
        return;
    }
    for (int i = startFrom; i < num_of_lines; i++) {
        mat_elem temp = matrix[i][columnOneIndex];
        matrix[i][columnOneIndex] = matrix[i][columnTwoIndex];
        matrix[i][columnTwoIndex] = temp;
    }
}

static void swap_matrix_lines(mat_elem **matrix, int lineOneIndex, int lineTwoIndex,
                              int num_of_columns, int startFrom) {
    if (lineOneIndex == lineTwoIndex) {
        return;
    }
    for (int i = startFrom; i < num_of_columns; i++) {
        mat_elem temp = matrix[lineOneIndex][i];
        matrix[lineOneIndex][i] = matrix[lineTwoIndex][i];
        matrix[lineTwoIndex][i] = temp;
    }
}

void free_matrix(mat_elem **matrix, int lines_number) {
    for (int i = 0; i < lines_number; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

void print_matrix(mat_elem **matrix, int num_of_lines, int num_of_columns) {
    for (int i = 0; i < num_of_lines; i++) {
        for (int j = 0; j < num_of_columns; j++) {
            printf("%+Lf ", matrix[i][j]);
        }
        printf("\n");
    }
}

void subtract_lines(mat_elem **matrix, int line1, int line2, int num_of_columns) {
    for (int i = 0; i < num_of_columns; i++) {

    }
}

mat_elem **get_invertible_matrix(mat_elem **matrix, int dim) {
    for (int i = 0; i < dim; i++) {
        for (int j = dim; j < 2*dim; j++) {
            matrix[i][j] = (i == (j - dim));
        }
    }
    get_row_echelon_matrix_form(matrix, dim, 2*dim, 0, NULL, NULL);

    for (int i = 0; i < dim; i++) {
        mat_elem first_not_zero_in_line = matrix[i][i];
        if (first_not_zero_in_line == 0.) {
            fprintf(stderr, "Error occurred while getting invertible matrix\n");
            exit(1);
        }

        for (int j = i; j < 2*dim; j++) {
            matrix[i][j] = matrix[i][j] / first_not_zero_in_line;
        }
    }

    for (int subtrahend_line_i = dim - 1; subtrahend_line_i >= 0; subtrahend_line_i--) {
        for (int minuend_line_i = 0; minuend_line_i < subtrahend_line_i; minuend_line_i++) {
            mat_elem scale = matrix[minuend_line_i][subtrahend_line_i];
            // subtract lines
            for (int column_i = 0; column_i < 2*dim; column_i++) {
                matrix[minuend_line_i][column_i] -= scale*matrix[subtrahend_line_i][column_i];
            }
        }
    }

    mat_elem **result = malloc(dim * sizeof(*matrix));
    for (int i = 0; i < dim; i++) {
        result[i] = malloc(dim * sizeof(**matrix));
        for (int j = 0; j < dim; j++) {
            result[i][j] = matrix[i][j+dim];
        }
    }
    return result;
}

mat_elem determinant(mat_elem **matrix, int dim) {
    unsigned num_of_transpos = 0;
    get_row_echelon_matrix_form(matrix, dim, dim, 1, NULL, &num_of_transpos);
    mat_elem det = 1;
    for (int i = 0; i < dim; i++) {
        det *= matrix[i][i];
    }
    return ((num_of_transpos & 1) ? -1 : 1) * det;
}

void print(mat_elem **matrix, int n, int m)
{
    int i, j;
    for (i = 0; i < n; ++i)
    {
        for (j = 0; j < m; ++j){
            if (j == m - 1) {
                printf("= %.2Lf,", matrix[i][j]);
            } else {
                printf("%c%.2Lf*x_%d ", matrix[i][j] >= 0 ? '+' : '-', fabsl(matrix[i][j]), j+1);
            }

        }

        printf("\n");
    }
}

int get_row_echelon_matrix_form(mat_elem **matrix, int num_of_lines, int num_of_columns,
                                int optimize, int *column_transpos, unsigned *num_of_transpos) {
    if (column_transpos) {
        for (int i = 0; i < num_of_lines; i++) {
            column_transpos[i] = i;
        }
    }
    for (int i = 0; i < num_of_lines - 1; i++) {
        if (optimize) {
            // Searching for column index of non zero element in line with index i
            int pivotColumnIndex = -1;
            for (int f = i; f < num_of_lines; f++) {
                if (fabsl(matrix[i][f]) >= eps &&
                    (pivotColumnIndex == -1 || fabsl(matrix[i][f]) > fabsl(matrix[i][pivotColumnIndex]))) {
                    pivotColumnIndex = f;
                }
            }
            if (pivotColumnIndex == -1) {
                return 0;
            }

            swap_matrix_columns(matrix, pivotColumnIndex, i, num_of_lines, 0);
            if (column_transpos) {
                int temp = column_transpos[i];
                column_transpos[i] = column_transpos[pivotColumnIndex];
                column_transpos[pivotColumnIndex] = temp;
            }
            if (num_of_transpos && pivotColumnIndex != i) {
                (*num_of_transpos)++;
            }
        } else {
            // Searching for line index of non zero element in column with index i
            int pivotLineIndex = -1;
            for (int f = i; f < num_of_lines; f++) {
                if (fabsl(matrix[f][i]) >= eps) {
                    pivotLineIndex = f;
                    break;
                }
            }
            if (pivotLineIndex == -1) {
                return 0;
            }

            swap_matrix_lines(matrix, pivotLineIndex, i, num_of_columns, i);
        }

        // Subtract from (([k] line) * [i][i])    ([i] line)*([k][i])
        for (int k = i + 1; k < num_of_lines; k++) {
            if (fabsl(matrix[k][i]) < eps) continue;

            mat_elem first_in_line = matrix[k][i];
            for (int j = i; j < num_of_columns; j++) {
                matrix[k][j] = matrix[k][j] - matrix[i][j] / matrix[i][i] * first_in_line;
            }
        }
    }
    return 1;
}
