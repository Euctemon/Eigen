#pragma once

#include <stdlib.h>
#include <stdbool.h>

struct Node {
    void* data;
    struct Node* next;
};

bool list_add(struct Node** head, void* data, size_t data_size);

void list_delete(struct Node** head, void (*free_data)(void*));