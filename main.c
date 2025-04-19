#include "eigenvalues.h"
#include "linkedlist.h"

#include <stdlib.h>
#include <crtdbg.h>

int main(void) {
    char filepathMatIn[] = "Data\\inputMatrix.txt";
    struct Matrix mat = mat_read(filepathMatIn);
    struct Node* head = NULL;

    struct Eigen_pair eigenpair = eigenpair_compute(&mat, 0.0001);
    struct Eigen_pair eigenpair2 = eigenpair_compute(&mat, 0.01);
    struct Eigen_pair eigenpair3 = eigenpair_compute(&mat, 0.001);
    
    bool err = false;

    err = list_add(&head, (void*)&eigenpair, sizeof(struct Eigen_pair));
    err = list_add(&head, (void*)&eigenpair2, sizeof(struct Eigen_pair));
    err = list_add(&head, (void*)&eigenpair3, sizeof(struct Eigen_pair));

    list_delete(&head, eigenpair_free);
    mat_free(mat);
    
    _CrtDumpMemoryLeaks();

    return 0;
}