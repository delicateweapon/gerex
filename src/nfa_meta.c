#include "nfa.h"
#include "util.h"

#include <stdlib.h>
#include <stdio.h>

NFA *nfa_create(void) {
    NFA *nfa = malloc(sizeof(*nfa));
    if (!nfa) {
        LOG_ERROR_HEAP_ALLOC("nfa");
    }
    nfa->state_start = state_create();
    nfa->state_end = state_create();
    return nfa;
}

NFA *nfa_from_symbol(char symbol) {
    NFA *nfa = nfa_create();
    state_append_transition(nfa->state_start, nfa->state_end, symbol);
    return nfa;
}

void nfa_destroy(NFA *nfa) {
    (void)nfa;
    // TODO
}
