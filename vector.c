#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "vector.h"

struct Vector vec_init(unsigned int dim) {
    size_t data_size = (size_t)dim * sizeof(double);
    struct Vector vec = { dim, malloc(data_size) };

    if (vec.data == NULL) {
        printf("could not allocate vector\n");
        exit(1);
    }

    return vec;
}

void vec_writeFile(struct Vector vec, char filepath[]) {
    FILE* stream = NULL;
    errno_t err = fopen_s(&stream, filepath, "w");

    if (err != 0) printf("writing to file: file was not opened\n");

    if (stream != NULL) {
        fprintf(stream, "%u\n", vec.dim);

        for (size_t i = 0; i < vec.dim; i++) {
            fprintf(stream, "%lf ", vec.data[i]);
        }

        fclose(stream);
    }
}

void vec_writeConsole(struct Vector vec) {
    for (size_t i = 0; i < vec.dim; i++) {
        printf("%lf ", vec.data[i]);
    }
}

void vec_free(struct Vector vec) {
    if (vec.data != NULL) {
        free(vec.data);
    }
}

void vec_setOnes(struct Vector* vec) {
    for (size_t i = 0; i < vec->dim; i++)
    {
        vec->data[i] = 1;
    }
}

double vec_dot(struct Vector* vec1, struct Vector* vec2) {
    double res = 0;
    for (size_t i = 0; i < vec1->dim; i++)
    {
        res = res + vec1->data[i] * vec2->data[i];
    }
    
    return res;
}

void vec_normalize(struct Vector* vec) {
    double norm = sqrt(vec_dot(vec, vec));

    for (size_t i = 0; i < vec->dim; i++)
    {
        vec->data[i] = vec->data[i] / norm;
    }

}

void vec_copy(struct Vector* vec_from, struct Vector* vec_to) {
    for (size_t i = 0; i < vec_from->dim; i++)
    {
        vec_to->data[i] = vec_from->data[i];
    }
}