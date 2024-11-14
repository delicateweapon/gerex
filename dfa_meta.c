#include "regulus.h"

#include <stdio.h>
#include <stdlib.h>

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

#define CAPACITY (1 << 6)
static NFA_State *considered_nfa_states[CAPACITY];
static size_t considered_nfa_states_count;

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

    considered_nfa_states[0] = nfa->begin;
    considered_nfa_states_count = 1;

    DFA_State *context_dfa;
    NFA_State *context_nfa;
    NFA_State *next;
    char symbol;

    for (size_t i = 0; i < considered_nfa_states_count; ++i) {
        if (g_DFA_State_count != i) {
            (void)DFA_State_create();
        }

        context_nfa = considered_nfa_states[i];
        context_dfa = g_DFA_States[i];

        for (size_t i = 0; i < g_symbols_count; ++i) {
            next = NFA_State_find_symbol_next(context_nfa);
            if (!next) {
                continue;
            }
        }
    }

    return dfa;
}
