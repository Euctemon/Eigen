#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include "matrix.h"
#include "vector.h"

// Structure holding eigenvalue and corresponding eigenvector.
struct EigenPair {
    double val;
    struct Vector* vec;
};

// Structure for singly linked list.
struct Node {
    struct EigenPair data;
    struct Node* next;
};

bool list_add(struct Node** head_pt, const struct EigenPair data);

void list_delete(struct Node** head);

// General matrix-vector multiplication.
void gemv(const struct Matrix* mat_pt, const struct Vector* vec_in_pt, struct Vector* vec_out_pt);

void eigenpair_write_console(const struct EigenPair eigen_pair);

void compute_deflate_vector(const struct Node* const* eigen_list, const struct Vector* vec_in_pt, struct Vector* vec_out_pt);

struct EigenPair eigenpair_compute(const struct Node* const* eigen_list, const struct Matrix* mat_pt, double tol, bool* converged_pt, bool* empty_image_pt);

void compute_and_write(const char filepath[], size_t num_of_eigenvals);

double small_rand();