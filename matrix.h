#pragma once

struct Matrix {
	unsigned int rows;
	unsigned int cols;
	double* data;
};

struct Matrix matrixInit(unsigned int rows, unsigned int cols);

void matrixFree(struct Matrix mat);