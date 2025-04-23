#pragma once

// Data stored as flexible array member.
struct Matrix {
	unsigned int dim;
	double data[];
};

struct Matrix* mat_init(size_t dim);

struct Matrix* mat_read(const char filepath[]);

void mat_write(const struct Matrix* mat_pt, const char filepath[]);

bool mat_symm_check(const struct Matrix* mat_pt);

