#include "eigenvalues.h"

static struct Node* get_tail(struct Node* head) {
	struct Node* current = head;

	while (current->next != NULL) {
		current = current->next;
	}

	return current;
}

bool list_add(struct Node** head_pt, const struct EigenPair data) {
	bool added = true;
	struct Node* node = (struct Node* )malloc(sizeof(struct Node));
	
	if (node == NULL) {
		added = false;
	}
	else {
		if (*head_pt == NULL) {
			*head_pt = node;
		}
		else {
			get_tail(*head_pt)->next = node;
		}
		node->data = data;
		node->next = NULL;
	}

	return added;
}

void list_delete(struct Node** head) {
	struct Node* current = *head;
	struct Node* next = *head;

	while (current != NULL)
	{
		next = current->next;
		
		if (current->data.vec != NULL) {
			free(current->data.vec);
		}

		free(current);
		current = next;
	}
	*head = NULL;
}

void gemv(const struct Matrix* const mat_pt, const struct Vector* vec_in_pt, struct Vector* vec_out_pt) {
	for (size_t i = 0; i < mat_pt->dim; i++)
	{
		vec_out_pt->data[i] = 0;
		for (size_t j = 0; j < mat_pt->dim; j++)
		{
			vec_out_pt->data[i] = vec_out_pt->data[i] + mat_pt->data[i * mat_pt->dim + j] * vec_in_pt->data[j];
		}
	}
}

void eigenpair_write_console(const struct EigenPair eigen_pair) {
    printf("eigenvalue approximation : %f\n", eigen_pair.val);
    printf("eigenvector approximation : ");
    vec_write_console(eigen_pair.vec);
}

void compute_deflate_vector(const struct Node* const* eigen_list, const struct Vector* vec_in_pt, struct Vector* vec_out_pt) {
	struct Vector* vec_temp_pt = vec_init(vec_in_pt->dim);
	const struct Node* eigen_pair = eigen_list == NULL ? NULL : (*eigen_list);
	double scale;

	vec_set_zeroes(vec_out_pt);

	while (eigen_pair != NULL)
	{
		scale = -1.0 * eigen_pair->data.val * vec_dot(eigen_pair->data.vec, vec_in_pt);
		
		vec_copy(eigen_pair->data.vec, vec_temp_pt);
		vec_smul(vec_temp_pt, scale);
		vec_add(vec_out_pt, vec_temp_pt);
		
		eigen_pair = eigen_pair->next;
	}

	free(vec_temp_pt);
}

void repair_sign(const struct Matrix* mat_pt, struct Vector* vec_pt, double* val_pt) {
	struct Vector* vec_out_pt = vec_init(vec_pt->dim);

	gemv(mat_pt, vec_pt, vec_out_pt);

	if (vec_dot(vec_pt, vec_out_pt) < 0)
	{
		vec_smul(vec_pt, -1);
		*val_pt = -1 * (*val_pt);
	}

	free(vec_out_pt);
}

double small_rand() {
	return (double)(rand() % 100) / 100;
}

struct Vector* get_image_space_vec(const struct Node* const* eigen_list, const struct Matrix* mat_pt, bool* empty_image_pt) {
	struct Vector* vec_current_pt = vec_init(mat_pt->dim);
	struct Vector* vec_next_pt = vec_init(mat_pt->dim);
	struct Vector* vec_deflate_pt = vec_init(mat_pt->dim);
	size_t iter_count = 0;
	
	vec_set_ones(vec_current_pt);

	while (true) {
		iter_count++;

		gemv(mat_pt, vec_current_pt, vec_next_pt);
		compute_deflate_vector(eigen_list, vec_current_pt, vec_deflate_pt);
		vec_add(vec_next_pt, vec_deflate_pt);
		
		if(iter_count > 10) {
			*empty_image_pt = true;
			break;
		}

		if (eq_num_zero_vec(vec_next_pt)) {
			vec_copy(vec_next_pt, vec_current_pt);
			vec_current_pt->data[iter_count % (vec_current_pt->dim - 1)] += small_rand();
			vec_normalize(vec_current_pt);

		}
		else {
			*empty_image_pt = false;
			break;
		}
	}
	
	free(vec_next_pt);
	free(vec_deflate_pt);

	return vec_current_pt;
}

struct EigenPair eigenpair_compute(const struct Node* const* eigen_list, const struct Matrix* mat_pt, double tol, bool* converged_pt, bool* empty_image_pt) {
	struct Vector* vec_current_pt = get_image_space_vec(eigen_list, mat_pt, empty_image_pt);
	struct Vector* vec_next_pt;
	struct Vector* vec_deflate_pt;
	struct EigenPair eigenpair = { 0,vec_current_pt };
	double val_current = 1;
    double val_next = 1;
    size_t iter_count = 0;

	if (*empty_image_pt) {
		return eigenpair;
	}

	vec_next_pt = vec_init(mat_pt->dim);
	vec_deflate_pt = vec_init(mat_pt->dim);

    while (true) {
        iter_count++;

        gemv(mat_pt, vec_current_pt, vec_next_pt);
        compute_deflate_vector(eigen_list, vec_current_pt, vec_deflate_pt);
    
        vec_add(vec_next_pt, vec_deflate_pt);
        vec_copy(vec_next_pt, vec_current_pt);
        vec_normalize(vec_current_pt);

        val_next = vec_dot(vec_current_pt, vec_next_pt);

        if (fabs(val_next - val_current) <= tol) {
			*converged_pt = true;
			break;
        }

		if (iter_count > 50) {
			*converged_pt = false;
			break;
		}

        val_current = val_next;
    }

	repair_sign(mat_pt, vec_current_pt, &val_current);
    eigenpair.val = val_current;
	eigenpair.vec = vec_current_pt;
	
    free(vec_next_pt);
    free(vec_deflate_pt);

    return eigenpair;
}

void compute_and_write(const char filepath[], size_t num_of_eigenvals) {
	struct Matrix* mat_pt = mat_read(filepath);
	struct Node* eigen_list = NULL;
	struct EigenPair current_pair;
	size_t iter_count = num_of_eigenvals < mat_pt->dim ? num_of_eigenvals : mat_pt->dim;
	bool converged = true;
	bool empty_image = true;

	if (!mat_is_symm(mat_pt)) {
		printf("the matrix is not symmetric");
		free(mat_pt);
		return;
	}
	
	for (size_t i = 0; i < iter_count; i++) {
		current_pair = eigenpair_compute(&eigen_list, mat_pt, 1.0E-5, &converged, &empty_image);
		
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