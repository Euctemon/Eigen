#pragma once

struct Vector {
	unsigned int dim;
	double* data;
};

struct Vector vec_init(unsigned int dim);

void vec_writeFile(struct Vector vec, char filepath[]);

void vec_writeConsole(struct Vector vec);

void vec_free(struct Vector vec);

void vec_setOnes(struct Vector* vec);

double vec_dot(struct Vector* vec1, struct Vector* vec2);

void vec_normalize(struct Vector* vec);

void vec_copy(struct Vector* vec_from, struct Vector* vec_to);