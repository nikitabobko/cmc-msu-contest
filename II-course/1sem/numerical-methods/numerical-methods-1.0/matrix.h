#ifndef NUMERICAL_METHODS_MATRIX_H
#define NUMERICAL_METHODS_MATRIX_H

#define eps 1e-6
#define mat_elem long double

/**
 * Get row echelon matrix form
 * @return 0 if cannot get get row echelon matrix form; otherwise 1;
 */
int get_row_echelon_matrix_form(mat_elem **matrix, int num_of_lines, int num_of_columns,
                                int optimize, int *column_transpos, unsigned *num_of_transpos);

/**
 * Compute invertible matrix
 * @param dim matrix dimension
 * @return invertible matrix
 */
mat_elem **get_invertible_matrix(mat_elem **matrix, int dim);

void free_matrix(mat_elem **matrix, int lines_number);

void print_matrix(mat_elem **matrix, int num_of_lines, int num_of_columns);

/**
 * Compute determinant
 * @param matrix matrix to compute determinant
 * @param dim matrix dimension
 * @return matrix determinant
 */
mat_elem determinant(mat_elem **matrix, int dim);

#endif
