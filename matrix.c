#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "matrix.h"

struct Matrix* mat_init(size_t dim) {
	struct Matrix* mat_pt = malloc(sizeof(struct Matrix) + dim * sizeof(double));
    
    if (mat_pt == NULL) {
        printf("could not allocate matrix\n");
        exit(1);
    }

    return mat_pt;
}

struct Matrix* mat_read(const char filepath[]) {
	struct Matrix* mat_pt = NULL;
	FILE* stream = NULL;
	errno_t err = fopen_s(&stream, filepath, "r");
	size_t dim;
	
	if (err != 0) {
		printf("reading file: file was not opened\n");
	}

	if (stream != NULL) {
		
		size_t i = 0;
		double temp = 0;

		fscanf_s(stream, "%zu", &dim);
		mat_pt = mat_init(dim);

		while (fscanf_s(stream, "%lf", &temp) == 1) {
			mat_pt->data[i] = temp;
			i++;
		}

		fclose(stream);
	}

	return mat_pt;
}

void mat_write_file(const struct Matrix* mat_pt, const char filepath[]) {
	FILE* stream = NULL;
	errno_t err = fopen_s(&stream, filepath, "w");

	if (err != 0) {
		printf("writing to file: file was not opened\n");
	}

	if (stream != NULL) {
		fprintf(stream, "%zu", mat_pt->dim);

		for (size_t i = 0; i < mat_pt->dim * mat_pt->dim; i++) {
			if (i % mat_pt->dim == 0) {
				fprintf(stream, "\n%lf", mat_pt->data[i]);
			}
			else {
				fprintf(stream, " %lf", mat_pt->data[i]);
			}
		}

		fclose(stream);
	}
}

void mat_write_console(const struct Matrix* mat_pt) {
	for (size_t i = 0; i < mat_pt->dim * mat_pt->dim; i++) {
		if (i % mat_pt->dim == 0) {
			printf("\n%lf", mat_pt->data[i]);
		}
		else {
			printf(" %lf", mat_pt->data[i]);
		}
	}
	printf("\n");
}

bool mat_symm_check(const struct Matrix* mat_pt) {
	for (size_t i = 0; i < mat_pt->dim; i++) {
		for (size_t j = i + 1; j < mat_pt->dim; j++) {
			if (mat_pt->data[i * mat_pt->dim + j] != mat_pt->data[j * mat_pt->dim + i]) {
				return false;
			}
		}
	}

	return true;
}