#include <stdio.h>

#include "eigenvalues.h"

void gemv(struct Matrix* mat, struct Vector* vecIn, struct Vector* vecOut) {
	for (size_t i = 0; i < mat->dim; i++)
	{
		vecOut->data[i] = 0;
		for (size_t j = 0; j < mat->dim; j++)
		{
			vecOut->data[i] = vecOut->data[i] + mat->data[i * mat->dim + j] * vecIn->data[j];
		}
	}
}
