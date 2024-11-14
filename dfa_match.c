#include "regulus.h"

bool DFA_match(DFA *dfa, const char *text)
{
    DFA_State *state;
    DFA_Move *move;
    size_t i, j;
    char c;

    state = dfa->begin;
    c = text[i = 0];
    while (c != '\0') {
        j = 0;
        while (j < state->moves_count) {
            move = &(state->moves[j]);
            if (move->symbol == c) {
                state = move->next;
                c = text[++i];
                break;
            }
            j++;
        }

        if (j == state->moves_count) {
            return false;
        }
    }

    for (i = 0; i < dfa->ends_count; ++i) {
        if (state == dfa->ends[i]) {
            return true;
        }
    }

    return false;
}
