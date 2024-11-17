#include "dfa.h"

DFA_State *g_start_state;

static size_t context_id;
static NFA_State *context_nfa;
static DFA_State *context_dfa;

static NFA_State *checkpoint_nfa_states[256];
static size_t checkpoint_nfa_state_count;
static char symbol;

int DFA_construct(NFA *nfa)
{
    checkpoint_nfa_states[0] = nfa->begin;
    checkpoint_nfa_state_count = 1;

    for (size_t i = 0; i < checkpoint_nfa_state_count; ++i) {
        if (g_DFA_State_count != i) {
            void *p = DFA_State_create();
            if (p == NULL) {
                return -1;
            }
        }

        context_dfa = &(g_DFA_State_storage[i]);
        context_nfa = checkpoint_nfa_states[i];

        for (size_t j = 0; j < g_symbol_count; ++j) {
            symbol = g_symbols[j];

            NFA_State *next = NFA_State_find_symbol_next(context_nfa, symbol);
            if (next == NULL) {
                continue;
            }

            bool found = false;
            size_t k = checkpoint_nfa_state_count;
            do {
                --k;
                if (next == checkpoint_nfa_states[k]) {
                    found = true;
                    break;
                }
            } while (k > 0);

            if (found) {
                int result_code = DFA_State_move_add(
                    context_dfa,
                    &(g_DFA_State_storage[k]),
                    symbol);

                if (result_code != 0) {
                    return -1;
                }
            } else {
                checkpoint_nfa_states[checkpoint_nfa_state_count++] = next;
                DFA_State *new = DFA_State_create();
                if (new == NULL) {
                    return -1;
                }

                int result_code = DFA_State_move_add(
                    context_dfa,
                    new,
                    symbol);

                if (result_code != 0) {
                    return -1;
                }
            }
        }

        if (context_dfa->move_count == 0 || context_nfa == nfa->end) {
            context_dfa->is_end = true;
        }
    }

    g_start_state = &(g_DFA_State_storage[0]);
    return 0;
}