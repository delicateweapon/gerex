#include "regulus.h"

NFA_State *NFA_State_find_symbol_next(NFA_State *state, char symbol)
{
    NFA_Move *move;
    NFA_State *maybe;

    if (state->moves_count == 0) {
        return NULL;
    }

    if (state->moves_count == 1) {
        move = &(state->moves[0]);
        if (move->symbol == symbol) {
            return move->next;
        }
        return NULL;
    }

    for (size_t i = 0; i < state->moves_count; ++i) {
        move = &(state->moves[i]);
        maybe = NFA_State_find_symbol_next(move->next, symbol);
        if (maybe != NULL) {
            return maybe;
        }
    }
    return NULL;
}
