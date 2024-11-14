#include "regulus.h"

#define EPSILON_MOVE(s1, s2) \
    NFA_State_move_add(s1, s2, SYMBOL_EPSILON)

uint8_t NFA_Op_precedence(NFA_Op op)
{
    switch (op) {
    case LPAREN:
    case RPAREN:
        return 0;
    case UNION:
        return 1;
    case CONCAT:
        return 2;
    case CLOSURE:
    case THREE_FOURTH_CLOSURE:
        return 3;
    }
}

NFA *NFA_union(NFA *nfa1, NFA *nfa2)
{
    NFA *result = NFA_create(true);

    EPSILON_MOVE(result->begin, nfa1->begin);
    EPSILON_MOVE(result->begin, nfa2->begin);

    EPSILON_MOVE(nfa1->end, result->end);
    EPSILON_MOVE(nfa2->end, result->end);

    return result;
}

NFA *NFA_concat(NFA *nfa1, NFA *nfa2)
{
    NFA *result = NFA_create(false);

    EPSILON_MOVE(nfa1->end, nfa2->begin);
    result->begin = nfa1->begin;
    result->end = nfa2->end;

    return result;
}

NFA *NFA_closure(NFA *nfa)
{
    NFA *result = NFA_create(true);

    EPSILON_MOVE(result->begin, nfa->begin);
    EPSILON_MOVE(result->begin, result->end);

    EPSILON_MOVE(nfa->end, nfa->begin);
    EPSILON_MOVE(nfa->end, result->end);

    return result;
}

NFA *NFA_three_fourth_closure(NFA *nfa)
{
    NFA *result = NFA_create(true);

    EPSILON_MOVE(result->begin, nfa->begin);
    /* yeah, it is indeed 3/4 */
    /* EPSILON_MOVE(result->begin, result->end); */

    EPSILON_MOVE(nfa->end, nfa->begin);
    EPSILON_MOVE(nfa->end, result->end);

    return result;
}
