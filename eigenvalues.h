#pragma once

#include <stdio.h>
#include <math.h>

#include "matrix.h"
#include "vector.h"
#include "linkedlist.h"

struct Eigen_pair {
    double eigenval;
    struct Vector eigenvec;
};


/// <summary>
/// General matrix-vector product between mat and vec_in, written to vec_out.
/// </summary>
void gemv(struct Matrix* mat, struct Vector* vec_in, struct Vector* vec_out);



void eigenpair_writeConsole(struct Eigen_pair eigenpair);

void eigenpair_free(struct Eigen_pair* eigenpair);

struct Eigen_pair eigenpair_compute(struct Matrix* mat, double tol);

//struct Eigenpair second_eigenpair_compute(struct Matrix* mat, double tol);