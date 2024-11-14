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

    static DFA_State *context_dfa;
    static NFA_State *context_nfa;
    static NFA_State *next;
    static char symbol;

    for (size_t i = 0; i < considered_nfa_states_count; ++i) {
        if (g_DFA_State_count - 1 != i) {
            (void)DFA_State_create();
        }

        context_nfa = considered_nfa_states[i];
        context_dfa = g_DFA_States[i];

        for (size_t j = 0; j < g_symbols_count; ++j) {
            symbol = g_symbols[j];

            next = NFA_State_find_symbol_next(context_nfa, symbol);
            if (!next) {
                continue;
            }

            bool found = false;
            size_t k = considered_nfa_states_count;
            do {
                --k;
                if (next == considered_nfa_states[k]) {
                    found = true;
                    break;
                }
            } while (k > 0);

            if (found) {
                DFA_State_move_add(
                    context_dfa,
                    g_DFA_States[k],
                    symbol);
            } else {
                considered_nfa_states[considered_nfa_states_count++] = next;
                DFA_State_move_add(
                    context_dfa,
                    DFA_State_create(),
                    symbol);
            }
        }

        if (context_dfa->moves_count == 0 || context_nfa == nfa->end) {
            DFA_add_end(dfa, context_dfa);
        }

        dfa->begin = g_DFA_States[0];
    }

    return dfa;
}
