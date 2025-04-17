#include <stdio.h>
#include "matrix.h"

int main(void) {

    char filepathIn[] = "Data\\inputMatrix.txt";
    char filepathOut[] = "Data\\outputMatrix.txt";

    struct Matrix mat;

    mat = matrixRead(filepathIn);
    matrixWrite(mat, filepathOut);



	return 0;
}