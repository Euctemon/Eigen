#pragma once

struct Matrix {
	unsigned int dim;
	double* data;
};

struct Matrix mat_init(unsigned int dim);

struct Matrix mat_read(char filepath[]);

void mat_free(struct Matrix mat);

void mat_write(struct Matrix mat, char filepath[]);

