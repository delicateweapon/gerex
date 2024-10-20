#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct state {
    struct state **epsilon_transitions;
    size_t epsilon_transitions_count;
    struct state *transitions[128];
    size_t transitions_count;
} State;

State *state_create(void);
void state_append_epsilon_transition(State *from, State *to);
void state_append_transition(State *from, State *to, char symbol);
void state_copy(State *dest, State *src);

typedef struct {
    State *state_start;
    State *state_end;
} NFA;

NFA *nfa_create(void);
void nfa_destroy(NFA *nfa);

NFA *nfa_closure(NFA *nfa);
NFA *nfa_concat(NFA *nfa1, NFA *nfa2);
NFA *nfa_concat_multiple(size_t count, ...);
NFA *nfa_union(NFA *nfa1, NFA *nfa2);
NFA *nfa_union_multiple(size_t count, ...);
