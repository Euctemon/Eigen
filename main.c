#include "eigenvalues.h"

#include <stdlib.h>
#include <crtdbg.h>

int main(void) {
    char filepathMatIn[] = "Data\\inputMatrix.txt";
    struct Matrix mat = mat_read(filepathMatIn);
    struct Node* head = NULL;
    bool err = false;

    struct EigenPair eigenpair = eigenpair_compute(mat,&head, 1.0E-5);
    err = list_add(&head, &eigenpair);
    struct EigenPair eigenpair2 = eigenpair_compute(mat,&head, 1.0E-5);
    err = list_add(&head, &eigenpair2);

    eigenpair_writeConsole(eigenpair2);

    list_delete(&head);
    mat_free(mat);
    
    _CrtDumpMemoryLeaks();

    return 0;
}