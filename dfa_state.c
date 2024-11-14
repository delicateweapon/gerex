#include "regulus.h"

#include <stdio.h>
#include <stdlib.h>

DFA_State *g_DFA_States[MAX_DFA_STATES];
size_t g_DFA_State_count = 0;

DFA_State *DFA_State_create(void)
{
    DFA_State *result;

    result = malloc(sizeof(*result));
    g_allocations[g_allocations_count++] = result;
    if (!result) {
        fprintf(stderr, "malloc error: %s\n", __func__);
        pthread_exit(NULL);
    }

    result->id = g_DFA_State_count;
    result->moves = malloc(sizeof(DFA_Move) * g_symbols_count);
    /* this causes double free */
    /* g_allocations[g_allocations_count++] = result->moves; */
    result->moves_count = 0;

    g_DFA_States[g_DFA_State_count++] = result;
    return result;
}

void DFA_State_move_add(DFA_State *state, DFA_State *next, char symbol)
{
    DFA_Move *temp;
    temp = realloc(state->moves, sizeof(NFA_Move) * (state->moves_count + 1));
    if (!temp) {
        fprintf(stderr, "realloc error: %s\n", __func__);
        pthread_exit(NULL);
    }

    state->moves = temp;
    state->moves[state->moves_count] = (DFA_Move) {
        .next = next,
        .symbol = symbol,
    };
    state->moves_count++;
}
