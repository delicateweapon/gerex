#include "regulus.h"

#include <stdlib.h>
#include <stdio.h>

size_t g_NFA_State_count = 0;

NFA_State *NFA_State_create(void)
{
    NFA_State *result;

    result = malloc(sizeof(*result));
    if (!result) {
        fprintf(stderr, "malloc error: %s\n", __func__);
        pthread_exit(NULL);
    }

    result->id = g_NFA_State_count;
    result->moves = NULL;
    result->moves_count = 0;

    g_NFA_State_count++;
    return result;
}

NFA *NFA_create(bool state_init)
{
    NFA *result;

    result = malloc(sizeof(*result));
    if (!result) {
        fprintf(stderr, "malloc error: %s\n", __func__);
        pthread_exit(NULL);
    }

    if (state_init) {
        result->begin = NFA_State_create();
        result->end = NFA_State_create();
    } else {
        result->begin = NULL;
        result->end = NULL;
    }

    return result;
}

inline NFA *NFA_from_symbol(char symbol)
{
    NFA *result = NFA_create(true);
    NFA_State_move_add(result->begin, result->end, symbol);
    return result;
}
