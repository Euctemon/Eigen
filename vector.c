#include <stdlib.h>
#include <stdio.h>

#include "vector.h"

struct Vector vectorInit(unsigned int dim) {
    size_t data_size = (size_t)dim * sizeof(double);
    struct Vector vec = { dim, malloc(data_size) };

    if (vec.data == NULL) {
        printf("could not allocate vector\n");
        exit(1);
    }

    return vec;
}

void vectorWrite(struct Vector vec, char filepath[]) {
    FILE* stream = NULL;
    errno_t err = fopen_s(&stream, filepath, "w");

    if (err != 0) printf("writing to file: file was not opened\n");

    if (stream != NULL) {
        fprintf(stream, "%u", vec.dim);

        for (size_t i = 0; i < vec.dim; i++) {
            fprintf(stream, " %lf", vec.data[i]);
        }

        fclose(stream);
    }
}

void vectorFree(struct Vector vec) {
    if (vec.data != NULL) {
        free(vec.data);
    }
}

void vectorSetOnes(struct Vector* vec) {
    for (size_t i = 0; i < vec->dim; i++)
    {
        vec->data[i] = 1;
    }
}

void vectorNormalize(struct Vector* vec) {
    double sum = 0;

    for (size_t i = 0; i < vec->dim; i++)
    {
        sum = sum + vec->data[i];
    }

    for (size_t i = 0; i < vec->dim; i++)
    {
        vec->data[i] = vec->data[i] / sum;
    }
}