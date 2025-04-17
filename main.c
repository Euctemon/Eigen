#include <stdio.h>
#include "eigenvalues.h"

int main(void) {

    char filepathMatIn[] = "Data\\inputMatrix.txt";
    /*char filepathMatOut[] = "Data\\outputMatrix.txt";*/
    char filepathVecOut[] = "Data\\outputVector.txt";

    struct Matrix mat;
    struct Vector vecIn;
    struct Vector vecOut;

    mat = matrixRead(filepathMatIn);
    vecIn = vectorInit(mat.dim);
    vecOut = vectorInit(mat.dim);

    vectorSetOnes(&vecIn);
       


    for (size_t i = 0; i < 10; i++)
    {
        gemv(&mat, &vecIn, &vecOut);
        vectorCopy(&vecOut, &vecIn);
        vectorNormalize(&vecIn);
        printf("%f\n", dot(&vecIn, &vecOut));
    }

    vectorWrite(vecIn, filepathVecOut);

	return 0;
}