#include "regulus.h"

#include <stdio.h>
#include <stdlib.h>

size_t g_DFA_State_count = 0;

DFA_State *DFA_State_create(void)
{
    DFA_State *result;

    result = malloc(sizeof(*result));
    g_allocations[g_allocations_count++] = result;
    if (!result) {
        fprintf(stderr, "malloc error: %s\n", __func__);        
        pthread_exit(NULL);
    }

    result->id = g_DFA_State_count;
    result->moves = NULL;
    result->moves_count = 0;

    g_DFA_State_count++;
    return result;
}

DFA *DFA_construct(NFA *nfa)
{
    DFA *dfa = malloc(sizeof(*dfa));
    g_allocations[g_allocations_count++] = dfa;
    if (!dfa) {
        fprintf(stderr, "malloc error: %s\n", __func__);
        pthread_exit(NULL);
    }

    return dfa;
}
