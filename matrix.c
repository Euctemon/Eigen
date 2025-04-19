#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "matrix.h"

struct Matrix mat_init(unsigned int dim) {
    size_t data_size = (size_t)dim * dim * sizeof(double);
    struct Matrix mat = {dim, malloc(data_size)};
    
    if (mat.data == NULL) {
        printf("could not allocate matrix\n");
        exit(1);
    }

    return mat;
}

void mat_free(struct Matrix mat) {
    if (mat.data != NULL) {
        free(mat.data);
		mat.data = NULL;
    }
}

struct Matrix mat_read(char filepath[]) {
	FILE* stream = NULL;
	errno_t err = fopen_s(&stream, filepath, "r");
	unsigned int dim;

	struct Matrix mat = { 0 };

	if (err != 0) printf("reading file: file was not opened\n");

	if (stream != NULL) {
		
		size_t i = 0;
		double temp = 0;

		fscanf_s(stream, "%u", &dim);
		mat = mat_init(dim);

		while (fscanf_s(stream, "%lf", &temp) == 1) {
			mat.data[i] = temp;
			i++;
		}

		fclose(stream);
	}

	return mat;
}

void mat_write(struct Matrix mat, char filepath[]) {
	FILE* stream = NULL;
	errno_t err = fopen_s(&stream, filepath, "w");

	if (err != 0) printf("writing to file: file was not opened\n");

	if (stream != NULL) {
		fprintf(stream, "%u", mat.dim);

		for (size_t i = 0; i < (size_t)mat.dim * mat.dim; i++) {
			if (i % mat.dim == 0) fprintf(stream, "\n%lf", mat.data[i]);
			else fprintf(stream, " %lf", mat.data[i]);
		}

		fclose(stream);
	}
}

bool isSymmetric(struct Matrix mat) {
	bool symmFlag = true;

	for (size_t i = 0; i < mat.dim; i++) {
		for (size_t j = i + 1; j < mat.dim; j++) {
			if (mat.data[i * mat.dim + j] != mat.data[j * mat.dim + i]) {
				symmFlag = false;
				goto end;
			}
		}
	}

	end: return symmFlag;
}