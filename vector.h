#pragma once
#pragma warning(disable : 4200)

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
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

// Dot product of a vector with self.
double vec_single_dot(const struct Vector* vec_pt);

// Dot product of non-overlapping vectors.
double vec_dot(const struct Vector* vec1_pt, const struct Vector* vec2_pt);

// Checks if the vector is numerically equal to the zero vector.
bool eq_num_zero_vec(const struct Vector* vec_pt);