#include "stack.h"
#include "util.h"

#include <stdlib.h>
#include <stdio.h>

void stack_append(Stack *stack, StackItem *item) {
    if (stack->count == stack->capacity) {
        StackItem *temp = realloc(stack->items, (stack->capacity * (3/2)) * stack->item_size);
        if (!temp) {
            LOG_ERROR_HEAP_ALLOC("temp");
            return;
        }
        stack->items = temp;
        stack->capacity *= 3/2;
    }
    stack->items[stack->count] = *item;
    stack->count++;
}

StackItem stack_view_top(Stack *stack) {
    StackItem result;
    if (stack->count == 0) {
        result.code = -1;
        return result;
    }
    result = stack->items[stack->count - 1];
    return result;
}

StackItem stack_pop(Stack *stack) {
    StackItem result;
    if (stack->count == 0) {
        result.code = -1;
        return result;
    }
    stack->count--;
    result = stack->items[stack->count];
    return result;
}
