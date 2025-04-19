#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "matrix.h"
#include "vector.h"

struct EigenPair {
    double eigenval;
    struct Vector eigenvec;
};

struct Node {
    struct EigenPair* data;
    struct Node* next;
};

void gemv(struct Matrix mat, struct Vector vec_in, struct Vector vec_out);

void eigenpair_writeConsole(struct EigenPair eigenpair);

void eigenpair_free(struct EigenPair eigenpair);

struct EigenPair eigenpair_compute(struct Matrix mat, struct Node** pair_list, double tol);

bool list_add(struct Node** head, struct EigenPair* data);

void list_delete(struct Node** head);

void compute_deflate_vector(struct Node** head, struct Vector vec_in, struct Vector vec_out);

