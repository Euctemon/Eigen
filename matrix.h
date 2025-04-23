#pragma once
#pragma warning(disable : 4200)

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>


// Data stored as flexible array member.
struct Matrix {
	size_t dim;
	double data[];
};

struct Matrix* mat_init(size_t dim);

struct Matrix* mat_read(const char filepath[]);

void mat_write_file(const struct Matrix* mat_pt, const char filepath[]);

void mat_write_console(const struct Matrix* mat_pt);

bool mat_is_symm(const struct Matrix* mat_pt);

