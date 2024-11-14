#include "regulus.h"

#include <stdio.h>
#include <stdlib.h>

DFA *DFA_construct(NFA *nfa)
{
    DFA *dfa = malloc(sizeof(*dfa));
    allocations[allocations_count++] = dfa;
    if (!dfa) {
        fprintf(stderr, "malloc error: %s\n", __func__);
        pthread_exit(NULL);
    }

    return dfa;
}
