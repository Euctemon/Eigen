#include "eigenvalues.h"

#include <stdlib.h>
#include <crtdbg.h>

int main(void) {
    compute_and_write("Data\\inputMatrix.txt", 2);
    _CrtDumpMemoryLeaks();

    return 0;
}