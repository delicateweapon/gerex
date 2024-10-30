#include "nfa.h"
#include "util.h"

#include <stdio.h>
#include <stdlib.h>

NFA *nfa_create(void) {
    NFA *nfa = malloc(sizeof(*nfa));
    if (nfa == NULL) {
        LOG_ERROR_HEAP_ALLOC("nfa");
    }
    nfa->start = state_create();
    nfa->end = state_create();
    return nfa;
}

inline NFA *nfa_from_symbol(int symbol) {
    NFA *nfa = nfa_create();
    state_add_move(nfa->start, nfa->end, symbol);
    return nfa;
}
