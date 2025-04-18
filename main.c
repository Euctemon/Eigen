#include <stdio.h>

#include "eigenvalues.h"



int main(void) {
    char filepathMatIn[] = "Data\\inputMatrix.txt";

    struct Matrix mat = mat_read(filepathMatIn);
    struct Eigenpair eigenpair = eigenpair_compute(&mat, 0.000001);

    eigenpair_writeConsole(eigenpair);
    eigenpair_free(eigenpair);

	return 0;
}