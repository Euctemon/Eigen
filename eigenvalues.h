#pragma once

#include <stdio.h>
#include <math.h>

#include "matrix.h"
#include "vector.h"

struct Eigenpair {
	double eigenval;
	struct Vector eigenvec;
};

/// <summary>
/// General matrix-vector product between mat and vec_in, written to vec_out.
/// </summary>
void gemv(struct Matrix* mat, struct Vector* vec_in, struct Vector* vec_out);

void eigenpair_writeConsole(struct Eigenpair eigenpair);

void eigenpair_free(struct Eigenpair eigenpair);

struct Eigenpair eigenpair_compute(struct Matrix* mat, double tol);