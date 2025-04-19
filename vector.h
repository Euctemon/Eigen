#pragma once

struct Vector {
	unsigned int dim;
	double* data;
};

struct Vector vec_init(unsigned int dim);

void vec_writeFile(struct Vector vec, char filepath[]);

void vec_writeConsole(struct Vector vec);

void vec_free(struct Vector vec);

void vec_setOnes(struct Vector vec);

void vec_setZeroes(struct Vector vec);

void vec_normalize(struct Vector vec);

void vec_copy(struct Vector vec_from, struct Vector vec_to);

void vec_smul(struct Vector vec, double scale);

/// <summary>
/// Overwrites the first vector.
/// </summary>
void vec_add(struct Vector vec1, struct Vector vec2);

double vec_dot(struct Vector vec1, struct Vector vec2);