#include <stdlib.h>
#include "matrix.h"

struct Matrix matrixInit(unsigned int rows, unsigned int cols) {
    size_t data_size = (size_t)rows * cols * sizeof(double);
    struct Matrix mat = {rows, cols, malloc(data_size)};
    
    if (mat.data == NULL) {
        printf("could not allocate matrix");
        exit(1);
    }

    return mat;
}

void matrixFree(struct Matrix mat) {
    if (mat.data != NULL) {
        free(mat.data);
    }
}