#include "eigenvalues.h"

struct Node* get_tail(struct Node* head) {
	struct Node* current = head;

	while (current->next != NULL) {
		current = current->next;
	}

	return current;
}

bool list_add(struct Node** head_pt, const struct EigenPair data) {
	bool added = true;
	struct Node* node = malloc(sizeof(struct Node));
	
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
		free(current->data.vec);
		free(current);
		current = next;
	}
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
		scale = eigen_pair->data.val * vec_dot(eigen_pair->data.vec, vec_in_pt);
		
		vec_copy(eigen_pair->data.vec, vec_temp_pt);
		vec_smul(vec_temp_pt, scale);
		vec_add(vec_out_pt, vec_temp_pt);
		
		eigen_pair = eigen_pair->next;
	}
    vec_smul(vec_out_pt, -1.0);
	free(vec_temp_pt);
}

struct EigenPair eigenpair_compute(const struct Node* const* eigen_list, const struct Matrix* mat_pt, double tol) {
    double val_current = 1;
    double val_next = 1;
    size_t iter_count = 0;

	struct EigenPair eigenpair = { 0,NULL };
    struct Vector* vec_current_pt = vec_init(mat_pt->dim);
    struct Vector* vec_next_pt = vec_init(mat_pt->dim);
    struct Vector* vec_deflate_pt = vec_init(mat_pt->dim);

    vec_set_ones(vec_current_pt);

    while (true) {
        iter_count++;

        gemv(mat_pt, vec_current_pt, vec_next_pt);
        compute_deflate_vector(eigen_list, vec_current_pt, vec_deflate_pt);
    
        vec_add(vec_next_pt, vec_deflate_pt);
        vec_copy(vec_next_pt, vec_current_pt);
        vec_normalize(vec_current_pt);

        val_next = vec_dot(vec_current_pt, vec_next_pt);

        if (fabs(val_next - val_current) <= tol || iter_count > 10) {
            break;
        }

        val_current = val_next;
    }

    eigenpair.val = val_current;
	eigenpair.vec = vec_current_pt;

    free(vec_next_pt);
    free(vec_deflate_pt);

    return eigenpair;
}