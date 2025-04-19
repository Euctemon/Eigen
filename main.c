#include "eigenvalues.h"

#include <stdlib.h>
#include <crtdbg.h>

int main(void) {
    char filepathMatIn[] = "Data\\inputMatrix.txt";
    struct Matrix mat = mat_read(filepathMatIn);
    struct Node* head = NULL;

    struct EigenPair eigenpair = eigenpair_compute(&mat, 1.0E-5);
    struct EigenPair eigenpair2 = eigenpair_compute(&mat, 0.01);
    struct EigenPair eigenpair3 = eigenpair_compute(&mat, 0.001);
    
    bool err = false;

    err = list_add(&head, (void*)&eigenpair, sizeof(struct EigenPair));
    err = list_add(&head, (void*)&eigenpair2, sizeof(struct EigenPair));
    err = list_add(&head, (void*)&eigenpair3, sizeof(struct EigenPair));

    list_delete(&head, eigenpair_free);
    mat_free(mat);
    
    _CrtDumpMemoryLeaks();

    return 0;
}