#include "eigenvalues.h"

struct State {
	struct Vector* vec_pt;
	enum { converged, not_converged };
};


enum convergence { converged, diverged, unknown };

struct VectorState {
	struct Vector* vec_pt;
	enum convergence state;
};

struct EigenpairState {
	struct Eigenpair pair;
	enum convergence state;
};



struct EigenArray* init_eigen_array(size_t num_of_vals, size_t vec_dim) {
	struct EigenArray* array_pt = malloc(sizeof(struct EigenArray) + num_of_vals * sizeof(struct Eigenpair));

	if (array_pt != NULL) {
		array_pt->capacity = num_of_vals;
		array_pt->filled = 0;

		for (size_t i = 0; i < num_of_vals; i++) {
			array_pt->data[i].eigenvec = vec_init(vec_dim);
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




// huh
void gemv(const struct Matrix* const mat_pt, const struct Vector* vec_in_pt, struct Vector* vec_out_pt) {
	for (size_t i = 0; i < mat_pt->dim; i++) {
		vec_out_pt->data[i] = 0;

		for (size_t j = 0; j < mat_pt->dim; j++) {
			vec_out_pt->data[i] = vec_out_pt->data[i] + mat_pt->data[i * mat_pt->dim + j] * vec_in_pt->data[j];
		}
	}
}





// get rid of slow linked list
//static struct Node* get_tail(struct Node* head) {
//	struct Node* current = head;
//
//	while (current->next != NULL) {
//		current = current->next;
//	}
//
//	return current;
//}
//
//bool list_add(struct Node** head_pt, const struct Eigenpair data) {
//	bool added = true;
//	struct Node* node = (struct Node*)malloc(sizeof(struct Node));
//	
//	if (node == NULL) {
//		added = false;
//	}
//	else {
//		if (*head_pt == NULL) {
//			*head_pt = node;
//		}
//		else {
//			get_tail(*head_pt)->next = node;
//		}
//		node->data = data;
//		node->next = NULL;
//	}
//
//	return added;
//}
//
//void list_delete(struct Node** head) {
//	struct Node* current = *head;
//	struct Node* next = *head;
//
//	while (current != NULL)
//	{
//		next = current->next;		
//		free(current->data.vec);
//		free(current);
//		current = next;
//	}
//	*head = NULL;
//}





void eigenpair_write_console(const struct Eigenpair pair) {
    printf("eigenvalue approximation : %f\n", pair.eigenval);
    printf("eigenvector approximation : ");
    vec_write_console(pair.eigenvec);
}

void compute_deflate_vector(const struct EigenArray* array_pt, const struct Vector* vec_in_pt, struct Vector* vec_out_pt) {
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

void repair_sign(const struct Matrix* mat_pt, struct Eigenpair pair) {
	struct Vector* vec_out_pt = vec_init(mat_pt->dim);

	gemv(mat_pt, pair.eigenvec, vec_out_pt);

	if (vec_dot(pair.eigenvec, vec_out_pt) < 0) {
		vec_smul(pair.eigenvec, -1);
		pair.eigenval = -pair.eigenval;
	}

	free(vec_out_pt);
}

struct VectorState get_image_vec(const struct EigenArray* array_pt, const struct Matrix* mat_pt) {
	struct Vector* vec_current_pt = vec_init(mat_pt->dim);
	struct Vector* vec_next_pt = vec_init(mat_pt->dim);
	struct Vector* vec_deflate_pt = vec_init(mat_pt->dim);
	enum convergence state = unknown;
	size_t iter_count = 0;
	
	vec_set_ones(vec_current_pt);

	while (true) {
		iter_count++;

		gemv(mat_pt, vec_current_pt, vec_next_pt);
		compute_deflate_vector(array_pt, vec_current_pt, vec_deflate_pt);
		vec_add(vec_next_pt, vec_deflate_pt);
		
		if (iter_count > 10) break; // cannot decide if diverges, so still unknown state

		if (eq_num_zero_vec(vec_next_pt)) {
			vec_copy(vec_next_pt, vec_current_pt);
			vec_current_pt->data[iter_count % (vec_current_pt->dim - 1)] += (double)(rand() % 100) / 100;
			vec_normalize(vec_current_pt);

		}
		else {
			state = converged;
			break;
		};
	}
	
	free(vec_next_pt);
	free(vec_deflate_pt);

	return (struct VectorState) { vec_current_pt, state };
}

struct EigenpairState eigenpair_compute(const struct Node* const* eigen_list, const struct Matrix* mat_pt, double tol) {
	struct VectorState image_vec_state = get_image_vec(eigen_list, mat_pt);
	struct Vector* vec_next_pt = vec_init(mat_pt->dim);
	struct Vector* vec_deflate_pt = vec_init(mat_pt->dim);
	struct Eigenpair eigenpair = { 1,image_vec_state.vec_pt };
	enum convergence state = unknown;
    double val_next = 1;
    size_t iter_count = 0;

	if (image_vec_state.state == unknown) {
		return (struct EigenpairState) { eigenpair, state };
	}

    while (true) {
        iter_count++;
		gemv(mat_pt, eigenpair.eigenvec, vec_next_pt);
        compute_deflate_vector(eigen_list, eigenpair.eigenvec, vec_deflate_pt);
        vec_add(vec_next_pt, vec_deflate_pt);
		vec_copy(vec_next_pt, eigenpair.eigenvec);
		vec_normalize(eigenpair.eigenvec);
        val_next = vec_dot(eigenpair.eigenvec, vec_next_pt);

        if (fabs(eigenpair.eigenval - val_next) <= tol) {
			state = converged;
			break;
        }

		if (iter_count > 50) {
			state = diverged;
			break;
		}

        eigenpair.eigenval = val_next;
    }

	repair_sign(mat_pt, eigenpair);
    free(vec_next_pt);
    free(vec_deflate_pt);

	return (struct EigenpairState) { eigenpair, state };
}


// zitra
void compute_and_write(const char filepath[], size_t num_of_eigenvals) {
	struct Matrix* mat_pt = mat_read(filepath);
	struct Node* eigen_list = NULL;
	struct Eigenpair current_pair;
	size_t iter_count = num_of_eigenvals < mat_pt->dim ? num_of_eigenvals : mat_pt->dim;
	bool converged = true;
	bool empty_image = true;

	if (!mat_is_symm(mat_pt)) {
		printf("the matrix is not symmetric");
		free(mat_pt);
		return;
	}
	
	for (size_t i = 0; i < iter_count; i++) {
		current_pair = eigenpair_compute(&eigen_list, mat_pt, 1E-8, &converged, &empty_image);
		
		list_add(&eigen_list, current_pair);
		if (empty_image) {
			printf("image space of matrix at deflation level %zu is numerically empty\n", i);
			break;
		}

		if (converged) {
			printf("the computed pair number %zu: \n", i+1);
			eigenpair_write_console(current_pair);
			printf("\n");
		}
		else {
			printf("the computation of %zu. eigenvalue did not converge\n", i+1);
			break;
		}
	}

	free(mat_pt);
	list_delete(&eigen_list);
}