#include "nfa.h"

static char symbol;

static inline State *state_next(State *state) {
    State *maybe = state->transitions[(int) symbol];
    if (maybe) {
        return maybe;
    }
    for (size_t i = 0; i < state->epsilon_transitions_count; ++i) {
        maybe = state_next(state->epsilon_transitions[i]);
        if (maybe) {
            return maybe;
        }
    }
    return NULL;
}

bool nfa_match(NFA *nfa, const char *text) {
    State *state = NULL;
    size_t i;

    i = 0;
    symbol = text[i];
    state = nfa->state_start;

    while (symbol != '\0') {
        state = state_next(state);
        if (!state) {
            return false;
        }
        symbol = text[++i];
        if (state == nfa->state_end) {
            if (symbol != '\0') {
                return false;
            } else {
                return true;
            }
        }
    }
    return false;
}
