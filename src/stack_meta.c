#include "stack.h"
#include "util.h"

#include <stdio.h>
#include <stdlib.h>

Stack *stack_create(size_t item_size, size_t capacity) {
    Stack *stack = malloc(sizeof(*stack));
    if (!stack) {
        LOG_ERROR_HEAP_ALLOC("stack");
        return NULL;
    }
    stack->items = malloc(item_size * capacity);
    if (!stack->items) {
        LOG_ERROR_HEAP_ALLOC("stack->items");
        return NULL;
    }
    stack->capacity = capacity;
    stack->count = 0;
    return stack;
}

inline void stack_destroy(Stack *stack) {
    free(stack->items);
    free(stack);
}
