#pragma once

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <stdio.h>

#include "matrix.h"
#include "vector.h"

struct Eigenpair {
	double eigenval;
	struct Vector* eigenvec;
};

struct EigenArray {
	size_t capacity;
	size_t filled;
	struct Eigenpair data[];
};

enum Convergence { converged, diverged, unknown };

struct EigenArray* init_eigen_array(size_t num_of_vals, size_t vec_dim);

void clear_eigen_array(struct EigenArray* array_pt);

enum Convergence fill_eigen_array(struct EigenArray* array_pt, const struct Matrix* mat_pt, double tol);

void compute_and_write(const char filepath[], size_t num_of_vals);