#include "nfa.h"
#include "util.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

State *state_create(void) {
    State *state = malloc(sizeof(*state));
    if (!state) {
        LOG_ERROR_HEAP_ALLOC("state");
        return NULL;
    }
    state->epsilon_transitions = NULL;
    memset(state->transitions, NULL, sizeof(state->transitions));
    return state;
}

void state_append_epsilon_transition(State *from, State *to) {
    State **temp =
        realloc(from->epsilon_transitions,
                sizeof(State *) * (from->epsilon_transitions_count + 1));
    if (!temp) {
        LOG_ERROR_HEAP_ALLOC("temp");
        return;
    }
    from->epsilon_transitions = temp;
    from->epsilon_transitions[from->epsilon_transitions_count] = to;
    from->epsilon_transitions_count++;
}

void state_append_transition(State *from, State *to, char symbol) {
    from->transitions[(int)symbol] = to;
    from->transitions_count++;
}

void state_copy(State *dest, State *src) {
    dest->epsilon_transitions =
        realloc(src->epsilon_transitions,
                src->epsilon_transitions_count * sizeof(State *));
    if (!dest->epsilon_transitions) {
        LOG_ERROR_HEAP_ALLOC("dest->epsilon_transitions");
        return;
    }
    dest->epsilon_transitions_count = src->epsilon_transitions_count;
    memcpy(dest->transitions, src->transitions, sizeof(src->transitions));
    dest->transitions_count = src->epsilon_transitions_count;
}
