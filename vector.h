#pragma once

struct Vector {
	unsigned int dim;
	double* data;
};

struct Vector vectorInit(unsigned int dim);

void vectorWrite(struct Vector vec, char filepath[]);

void vectorFree(struct Vector vec);

void vectorSetOnes(struct Vector* vec);

double dot(struct Vector* vec1, struct Vector* vec2);

void vectorNormalize(struct Vector* vec);

void vectorCopy(struct Vector* vec1, struct Vector* vec2);