#include "nfa.h"

#include <stdio.h>
#include <stdlib.h>

NFA *NFA_create(bool states_init) 
{
    NFA *result;

    result = malloc(sizeof(*result));
    if (result == NULL) {
        fprintf(stderr, "malloc error: %s\n", __func__);
        return NULL;
    }

    if (states_init) {
        result->begin = NFA_State_create();
        if (result->begin == NULL) {
            return NULL;
        }
        result->end = NFA_State_create();
        if (result->end == NULL) {
            return NULL;
        }
    } else {
        result->begin = NULL;
        result->end = NULL;
    }

    return result;
}

char *g_symbols;
size_t g_symbols_capacity;
size_t g_symbol_count;

#define INIT_CAPACITY 16
static bool initialized = false;

static inline int initialize(void)
{
    g_symbols = malloc(sizeof(char) * INIT_CAPACITY);
    if (g_symbols == NULL) {
        fprintf(stderr, "malloc error: failed to allocate storage for symbols\n");
        return -1;
    }

    g_symbols_capacity = INIT_CAPACITY;
    g_symbol_count = 0;

    initialized = true;
    return 0;
}

NFA *NFA_from_symbol(char symbol) 
{
    for (size_t i = 0; i < g_symbol_count; ++i) {
        if (symbol == g_symbols[i]) {
            goto after_g_symbols;
        }
    }

    if (g_symbols_capacity == g_symbol_count) {
        if (!initialized) {
            int result_code = initialize();
            if (result_code != 0) {
                return NULL;
            }
        }

        char *temp = realloc(g_symbols, sizeof(char) * (g_symbols_capacity * 1.5));
        if (temp == NULL) {
            fprintf(stderr, "realloc error: failed to increase symbols capacity\n");
            return NULL;
        }

        g_symbols = temp;
    }

    g_symbols[g_symbol_count++] = symbol;

after_g_symbols:

    NFA *result = NFA_create(true);

    int result_code = NFA_State_move_add(result->begin, result->end, symbol);
    if (result_code != 0) {
        return NULL;
    }

    return result;
}
