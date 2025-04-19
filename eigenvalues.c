#include <stdio.h>
#include <stdbool.h>

#include "eigenvalues.h"

void gemv(struct Matrix mat, struct Vector vec_in, struct Vector vec_out) {
	for (size_t i = 0; i < mat.dim; i++)
	{
		vec_out.data[i] = 0;
		for (size_t j = 0; j < mat.dim; j++)
		{
			vec_out.data[i] = vec_out.data[i] + mat.data[i * mat.dim + j] * vec_in.data[j];
		}
	}
}

void eigenpair_writeConsole(struct EigenPair eigenpair) {
    printf("eigenvalue approximation : %f\n", eigenpair.eigenval);
    
    printf("eigenvector approximation : ");
    vec_writeConsole(eigenpair.eigenvec);
    printf("\n");
}

void eigenpair_free(struct EigenPair eigenpair) {
    vec_free(eigenpair.eigenvec);
}

struct EigenPair eigenpair_compute(struct Matrix mat, double tol) {
    double val_current = 1;
    double val_next = 1;
    size_t iter_count = 0;

    struct EigenPair eigenpair = {0, vec_init(mat.dim)};
    struct Vector vec_current = vec_init(mat.dim);
    struct Vector vec_next = vec_init(mat.dim);

    vec_setOnes(vec_current);

    while (true) {
        iter_count++;

        gemv(mat, vec_current, vec_next);
        
        vec_copy(vec_next, vec_current);
        vec_normalize(vec_current);
        val_next = vec_dot(vec_current, vec_next);

        if (fabs(val_next-val_current) <= tol || iter_count > 100) {
            break;
        }

        val_current = val_next;
    }

    eigenpair.eigenval = val_current;
    vec_copy(vec_current, eigenpair.eigenvec);

    vec_free(vec_current);
    vec_free(vec_next);

    return eigenpair;
}



struct Node* get_tail(struct Node* head) {
	struct Node* current = head;

	while (current->next != NULL) {
		current = current->next;
	}

	return current;
}

bool list_add(struct Node** head, struct EigenPair* data) {
	bool added = true;
	struct Node* node = malloc(sizeof(struct EigenPair*));

	if (node == NULL) {
		added = false;
	}
	else {
		if (*head == NULL) {
			*head = node;
		}
		else {
			get_tail(*head)->next = node;
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
		eigenpair_free(*current->data);
		free(current);
		current = next;
	}
}