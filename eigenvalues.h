#pragma once

#include <stdio.h>
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

bool list_add(struct Node** head, void* data, size_t data_size);

void list_delete(struct Node** head, void (*free_data)(void*));

void gemv(struct Matrix mat, struct Vector vec_in, struct Vector vec_out);


void eigenpair_writeConsole(struct EigenPair eigenpair);

void eigenpair_free(struct EigenPair* eigenpair);

struct EigenPair eigenpair_compute(struct Matrix* mat, double tol);