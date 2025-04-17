#pragma once

struct Matrix {
	unsigned int dim;
	double* data;
};

struct Matrix matrixInit(unsigned int dim);

struct Matrix matrixRead(char filepath[]);

void matrixFree(struct Matrix mat);

void matrixWrite(struct Matrix mat, char filepath[]);

