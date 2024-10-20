#pragma once

#include "nfa.h"

typedef union {
    NFA *nfa;
    char symbol;
    int code;
} StackItem;

typedef struct {
    size_t capacity;
    size_t count;
    size_t item_size;
    StackItem *items;
} Stack;

Stack *stack_create(size_t item_size, size_t capacity);
void stack_destroy(Stack *stack);

void stack_append(Stack *stack, StackItem *item);
StackItem stack_view_top(Stack *stack);
StackItem stack_pop(Stack *stack);
