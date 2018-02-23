#include <stdlib.h>
#include <string.h>
#include "matrix.h"
#include "system_of_linear_equations_solver.h"

mat_elem *solve_system_of_linear_equations(mat_elem **matrix, int dim, int optimize) {
	// Getting row echelon matrix form
    int *column_transpos = malloc(dim * sizeof(*column_transpos));
    if (!column_transpos) {
        return NULL;
    }
    if (!get_row_echelon_matrix_form(matrix, dim, dim + 1, optimize, column_transpos, NULL)) {
        free(column_transpos);
        return NULL;
    }
	mat_elem *temp = malloc(dim * sizeof(*temp));
    if (!temp) {
        free(column_transpos);
        return NULL;
    }
	for (int i = 0; i < dim; i++) {
		temp[i] = 0.;
	}
	for (int i = dim - 1; i >= 0; i--) {
		mat_elem offset = 0.;
		for (int k = i + 1; k < dim; k++) {
			offset = offset + matrix[i][k] * temp[k];
		}
		temp[i] = (matrix[i][dim] - offset) / matrix[i][i];
	}
    mat_elem *result = malloc(dim * sizeof(*temp));
    if (!result) {
        free(temp);
        free(column_transpos);
        return NULL;
    }
    for (int i = 0; i < dim; i++) {
        result[column_transpos[i]] = temp[i];
    }
    free(column_transpos);
    free(temp);
	return result;
}