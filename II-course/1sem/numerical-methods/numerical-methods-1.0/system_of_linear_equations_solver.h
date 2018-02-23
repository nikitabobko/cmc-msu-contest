#ifndef SYSTEM_OF_LINEAR_EQUATIONS_SOLVER_MODULE
#define SYSTEM_OF_LINEAR_EQUATIONS_SOLVER_MODULE 1

#include "matrix.h"

/**
 * Solve system of linear equations
 * @param matrix The matrix
 * @param dim Main matrix dimension
 * @param optimize Whether to optimize main line choice
 * @return Array of n elements - solution of system_of_linear_equations or NULL if something failed
 */
mat_elem *solve_system_of_linear_equations(mat_elem **matrix, int dim, int optimize);

#endif
