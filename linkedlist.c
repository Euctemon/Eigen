#include "linkedlist.h"

struct Node* get_tail(struct Node* head) {
	struct Node* current = head;

	while (current->next != NULL) {
		current = current->next;
	}

	return current;
}

bool list_add(struct Node** head, void* data, size_t data_size) {
	bool added = true;
	struct Node* node = malloc(data_size);
	
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

void list_delete(struct Node** head, void (*free_data)(void*)) {
	struct Node* current = *head;
	struct Node* next = *head;

	while (current != NULL)
	{
		next = current->next;
		free_data(current->data);
		free(current);
		current = next;
	}
}

