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

static void DFA_add_end(DFA *dfa, DFA_State *end)
{
    if (dfa->ends_count == dfa->ends_capacity) {
        DFA_State **temp;
        temp = realloc(dfa->ends, sizeof(DFA_State *) * (dfa->ends_capacity * (3 / 2)));
        if (!temp) {
            fprintf(stderr, "realloc error: %s\n", __func__);
            pthread_exit(NULL);
        }
        dfa->ends = temp;
        dfa->ends_capacity *= (3 / 2);
    }

    dfa->ends[dfa->ends_count++] = end;
}

DFA *DFA_construct(NFA *nfa)
{
    DFA *dfa = malloc(sizeof(*dfa));
    g_allocations[g_allocations_count++] = dfa;
    if (!dfa) {
        fprintf(stderr, "malloc error: %s\n", __func__);
        pthread_exit(NULL);
    }

    dfa->ends = malloc(sizeof(DFA_State *) * 4);
    if (!dfa->ends) {
        fprintf(stderr, "malloc error: %s\n", __func__);
        pthread_exit(NULL);
    }
    dfa->ends_capacity = 4;

    return dfa;
}
