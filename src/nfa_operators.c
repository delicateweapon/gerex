#include "nfa.h"
#include "util.h"

#include <stdarg.h>
#include <stdio.h>

NFA *nfa_closure(NFA *nfa) {
    NFA *result = nfa_create();
    state_append_epsilon_transition(result->state_start, result->state_end);
    state_append_epsilon_transition(result->state_start, nfa->state_start);
    state_append_epsilon_transition(nfa->state_end, result->state_end);
    state_append_epsilon_transition(nfa->state_end, nfa->state_start);
    return result;
}

NFA *nfa_concat(NFA *nfa1, NFA *nfa2) {
    state_copy(nfa1->state_end, nfa2->state_start);
    nfa2->state_start = nfa1->state_end;

    NFA *result = nfa_create();
    result->state_start = nfa1->state_start;
    result->state_end = nfa2->state_end;
    return result;
}

NFA *nfa_concat_multiple(size_t count, ...) {
    if (count < 2) {
        LOG_ERROR("count must be greater than 2");
        return NULL;
    }

    va_list args;
    va_start(args, count);

    NFA *nfa1, *nfa2;
    nfa1 = va_arg(args, NFA *);

    NFA *result = nfa_create();
    result->state_start = nfa1->state_start;

    for (size_t i = 1; i < count; ++i) {
        nfa2 = va_arg(args, NFA *);
        nfa_concat(nfa1, nfa2);
        nfa1 = nfa2;
    }

    va_end(args);

    result->state_end = nfa1->state_end;
    return result;
}

NFA *nfa_concat_array(size_t count, NFA **nfas) {
    if (count < 2) {
        LOG_ERROR("count must be greater than 2");
        return NULL;
    }

    NFA *nfa1, *nfa2;
    nfa1 = nfas[0];

    NFA *result = nfa_create();
    result->state_start = nfa1->state_start;

    for (size_t i = 1; i < count; ++i) {
        nfa2 = nfas[i];
        nfa_concat(nfa1, nfa2);
        nfa1 = nfa2;
    }

    result->state_end = nfa1->state_end;
    return result;
}

NFA *nfa_union(NFA *nfa1, NFA *nfa2) {
    NFA *result = nfa_create();
    state_append_epsilon_transition(result->state_start, nfa1->state_start);
    state_append_epsilon_transition(nfa1->state_end, result->state_end);
    state_append_epsilon_transition(result->state_start, nfa2->state_start);
    state_append_epsilon_transition(nfa2->state_end, result->state_end);
    return result;
}

NFA *nfa_union_multiple(size_t count, ...) {
    if (count < 2) {
        LOG_ERROR("count must be greater than 2");
        return NULL;
    }

    va_list args;
    va_start(args, count);

    NFA *result = nfa_create();
    NFA *nfa;

    for (size_t i = 0; i < count; ++i) {
        nfa = va_arg(args, NFA *);
        state_append_epsilon_transition(result->state_start, nfa->state_start);
        state_append_epsilon_transition(nfa->state_end, result->state_end);
    }

    va_end(args);
    return result;
}

NFA *nfa_union_array(size_t count, NFA **nfas) {
    if (count < 2) {
        LOG_ERROR("count must be greater than 2");
        return NULL;
    }

    NFA *result = nfa_create();
    NFA *nfa;

    for (size_t i = 0; i < count; ++i) {
        nfa = nfas[i];
        state_append_epsilon_transition(result->state_start, nfa->state_start);
        state_append_epsilon_transition(nfa->state_end, result->state_end);
    }

    return result;
}
