#include "eigenvalues.h"

struct EigenArray* init_eigen_array(size_t num_of_vals, size_t vec_dim) {
	struct EigenArray* array_pt = malloc(sizeof(struct EigenArray) + num_of_vals * sizeof(struct Eigenpair));

	if (array_pt != NULL) {
		array_pt->capacity = num_of_vals;
		array_pt->filled = 0;

		for (size_t i = 0; i < num_of_vals; i++) {
			array_pt->data[i].eigenvec = vec_init(vec_dim);
			array_pt->data[i].eigenval = NAN;
		}
	}

	return array_pt;
}

void clear_eigen_array(struct EigenArray* array_pt) {
	for (size_t i = 0; i < array_pt->capacity; i++) {
		free(array_pt->data[i].eigenvec);
	}

	free(array_pt);
}

static void print_eigen_array(struct EigenArray* array_pt, size_t desired_vals) {
	for (size_t i = 0; i < desired_vals; i++) {
		printf("eigenvalue approximation : %f\n", array_pt->data[i].eigenval);
		printf("eigenvector approximation : ");
		vec_write_console(array_pt->data[i].eigenvec);
	}
}

static void gemv(const struct Matrix* const mat_pt, const struct Vector* vec_in_pt, struct Vector* vec_out_pt) {
	for (size_t i = 0; i < mat_pt->dim; i++) {
		vec_out_pt->data[i] = 0;

		for (size_t j = 0; j < mat_pt->dim; j++) {
			vec_out_pt->data[i] = vec_out_pt->data[i] + mat_pt->data[i * mat_pt->dim + j] * vec_in_pt->data[j];
		}
	}
}

static void compute_deflate_vector(const struct EigenArray* array_pt, const struct Vector* vec_in_pt, struct Vector* vec_out_pt) {
	struct Vector* vec_temp_pt = vec_init(vec_in_pt->dim);
	double scale;

	vec_set_zeroes(vec_out_pt);
	
	for (size_t i = 0; i < array_pt->filled; i++) {
		scale = -array_pt->data[i].eigenval * vec_dot(array_pt->data[i].eigenvec, vec_in_pt);
		vec_copy(array_pt->data[i].eigenvec, vec_temp_pt);
		vec_smul(vec_temp_pt, scale);
		vec_add(vec_out_pt, vec_temp_pt);
	}

	free(vec_temp_pt);
}

static void repair_sign(const struct Matrix* mat_pt, struct Eigenpair* pair) {
	struct Vector* vec_out_pt = vec_init(mat_pt->dim);

	gemv(mat_pt, pair->eigenvec, vec_out_pt);

	if (vec_dot(pair->eigenvec, vec_out_pt) < 0) {
		vec_smul(pair->eigenvec, -1);
		pair->eigenval = -pair->eigenval;
	}

	free(vec_out_pt);
}

static enum Convergence fill_image_vec(const struct EigenArray* array_pt, const struct Matrix* mat_pt, struct Vector* vec_image_pt) {
	struct Vector* vec_next_pt = vec_init(mat_pt->dim);
	struct Vector* vec_deflate_pt = vec_init(mat_pt->dim);
	enum Convergence state = unknown;
	size_t iter_count = 0;
	
	vec_set_ones(vec_image_pt);

	while (true) {
		iter_count++;

		gemv(mat_pt, vec_image_pt, vec_next_pt);
		compute_deflate_vector(array_pt, vec_image_pt, vec_deflate_pt);
		vec_add(vec_next_pt, vec_deflate_pt);
		
		if (iter_count > 10) break;

		if (eq_num_zero_vec(vec_next_pt)) {
			vec_copy(vec_next_pt, vec_image_pt);
			vec_image_pt->data[iter_count % (vec_image_pt->dim - 1)] += (double)(rand() % 100) / 100;
			vec_normalize(vec_image_pt);

		}
		else {
			state = converged;
			break;
		};
	}
	
	free(vec_next_pt);
	free(vec_deflate_pt);

	return state;
}

enum Convergence fill_eigen_array(struct EigenArray* array_pt, const struct Matrix* mat_pt, double tol) {
	enum Convergence state = unknown;
	struct Vector* vec_arr[3] = { NULL };
	double val_current = 0;
	double val_next = 1;
	size_t current_iter = 0;

	for (size_t i = 0; i < 3; i++) {
		vec_arr[i] = vec_init(mat_pt->dim);
	}

	for (size_t i = 0; i < array_pt->capacity; i++) {
		current_iter = 0;

		if (fill_image_vec(array_pt, mat_pt, vec_arr[0]) == unknown) {
			state = unknown;
			goto free_vec_arr;
		}

		while (true) {
			current_iter++;
			gemv(mat_pt, vec_arr[0], vec_arr[1]);
			compute_deflate_vector(array_pt, vec_arr[0], vec_arr[2]);
			vec_add(vec_arr[1], vec_arr[2]);
			vec_copy(vec_arr[1], vec_arr[0]);
			vec_normalize(vec_arr[0]);
			val_next = vec_dot(vec_arr[0], vec_arr[1]);

			if (fabs(val_current - val_next) <= tol) {
				array_pt->filled++;
				array_pt->data[i].eigenval = val_current;
				vec_copy(vec_arr[0], array_pt->data[i].eigenvec);
				repair_sign(mat_pt, &array_pt->data[i]);
				state = converged;
				break;
			}

			if (current_iter > 100) {
				state = diverged;
				goto free_vec_arr;
			}

			val_current = val_next;
		}
	}

	free_vec_arr : for (size_t i = 0; i < 3; i++) {
		free(vec_arr[i]);
	}

	return state;
}

void compute_and_write(const char filepath[], size_t num_of_vals) {
	struct Matrix* mat_pt = mat_read(filepath);
	struct EigenArray* pairs = init_eigen_array(num_of_vals < mat_pt->dim ? num_of_vals : mat_pt->dim, mat_pt->dim);

	if (mat_pt == NULL || !mat_is_symm(mat_pt)) {
		printf("the matrix is not symmetric");
		free(mat_pt);
		return;
	}

	switch (fill_eigen_array(pairs, mat_pt, 1E-6)) {
		case unknown:
			print_eigen_array(pairs, pairs->filled);
			printf("image space of matrix at deflation level %zu is numerically empty\n", pairs->filled);
			break;
		case diverged:
			print_eigen_array(pairs, pairs->filled);
			printf("the computation of %zu. eigenvalue did not converge\n", pairs->filled);
		case converged:
			print_eigen_array(pairs, pairs->capacity);
			printf("computation is completed\n");
		default:
			printf("this should not have happened\n");
			break;
	}

	free(mat_pt);
	clear_eigen_array(pairs);
}