#pragma once

#include "matrix.h"
#include "vector.h"

/// <summary>
/// General matrix-vector product between mat and vec1, written to vec2.
/// </summary>
void gemv(struct Matrix* mat, struct Vector* vecIn, struct Vector* vecOut);