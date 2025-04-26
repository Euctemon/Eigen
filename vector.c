#include "vector.h"
#define TOL 1E-12

struct Vector* vec_init(size_t dim) {
    struct Vector* vec_pt = malloc(sizeof(struct Vector) + dim * sizeof(double));
    
    if (vec_pt == NULL) {
        printf("could not allocate vector\n");
        exit(1);
    }
    else {
        vec_pt->dim = dim;
    }

    return vec_pt;
}

void vec_write_file(const struct Vector* vec_pt, const char filepath[]) {
    FILE* stream = NULL;
    errno_t err = fopen_s(&stream, filepath, "w");

    if (err != 0) {
        printf("writing to file: file was not opened\n");
    }

    if (stream != NULL) {
        fprintf(stream, "%zu\n", vec_pt->dim);

        for (size_t i = 0; i < vec_pt->dim; i++) {
            fprintf(stream, "%lf ", vec_pt->data[i]);
        }

        fclose(stream);
    }
}

void vec_write_console(const struct Vector* vec_pt) {
    for (size_t i = 0; i < vec_pt->dim; i++) {
        printf("%lf ", vec_pt->data[i]);
    }
    printf("\n");
}

void vec_set_zeroes(struct Vector* vec_pt) {
    for (size_t i = 0; i < vec_pt->dim; i++) {
        vec_pt->data[i] = 0;
    }
}

void vec_set_ones(struct Vector* vec_pt) {
    for (size_t i = 0; i < vec_pt->dim; i++) {
        vec_pt->data[i] = 1;
    }
}

void vec_normalize(struct Vector* vec_pt) {
    double norm = sqrt(vec_single_dot(vec_pt));

    if (norm < TOL) {
        return;
    }

    for (size_t i = 0; i < vec_pt->dim; i++) {
        vec_pt->data[i] = vec_pt->data[i] / norm;
    }
}

void vec_copy(const struct Vector* vec_from_pt, struct Vector* vec_to_pt) {
    memcpy(vec_to_pt, vec_from_pt, sizeof(struct Vector) + (vec_from_pt->dim * sizeof(double)));
}

void vec_smul(struct Vector* vec_pt, double scale) {
    for (size_t i = 0; i < vec_pt->dim; i++) {
        vec_pt->data[i] = vec_pt->data[i] * scale;
    }
}

void vec_add(struct Vector* vec1_pt, const struct Vector* vec2_pt) {
    for (size_t i = 0; i < vec1_pt->dim; i++) {
        vec1_pt->data[i] = vec1_pt->data[i] + vec2_pt->data[i];
    }
}

double vec_single_dot(const struct Vector* vec_pt) {
    double res = 0;

    for (size_t i = 0; i < vec_pt->dim; i++) {
        res = res + vec_pt->data[i] * vec_pt->data[i];
    }

    return res;
}

double vec_dot(const struct Vector* restrict vec1_pt, const struct Vector* restrict vec2_pt) {
    double res = 0;

    for (size_t i = 0; i < vec1_pt->dim; i++) {
        res = res + vec1_pt->data[i] * vec2_pt->data[i];
    }

    return res;
}

bool eq_num_zero_vec(const struct Vector* vec_pt) {
    return (vec_single_dot(vec_pt) < TOL) ? true : false;
}