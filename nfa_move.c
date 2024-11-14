#include "regulus.h"

#include <stdio.h>
#include <stdlib.h>

void NFA_State_move_add(NFA_State *state, NFA_State *next, char symbol)
{
    NFA_Move *temp;
    temp = realloc(state->moves, sizeof(NFA_Move) * (state->moves_count + 1));
    if (!temp) {
        fprintf(stderr, "realloc error: %s\n", __func__);
        pthread_exit(NULL);
    }

    state->moves = temp;
    state->moves[state->moves_count] = (NFA_Move) {
        .next = next,
        .symbol = symbol,
    };
    state->moves_count++;
}
