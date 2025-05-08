#pragma once
#pragma warning(disable : 4200)

#include <stdbool.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Data stored as flexible-array member.
struct Vector {
	size_t dim;
	double data[];
};

struct Vector* vec_init(size_t dim);

void vec_write_file(const struct Vector* vec_pt, const char filepath[]);

void vec_write_console(const struct Vector* vec_pt);

void vec_set_zeroes(struct Vector* vec_pt);

void vec_set_ones(struct Vector* vec_pt);

void vec_normalize(struct Vector* vec_pt);

void vec_copy(const struct Vector* vec_from_pt, struct Vector* vec_to_pt);

void vec_smul(struct Vector* vec_pt, double scale);

// Overwrites the first vector.
void vec_add(struct Vector* vec1_pt, const struct Vector* vec2_pt);

double vec_dot(const struct Vector* vec1_pt, const struct Vector* vec2_pt);

bool eq_num_zero_vec(const struct Vector* vec_pt);