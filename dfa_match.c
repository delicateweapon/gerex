#include "regulus.h"

bool DFA_match(DFA *dfa, const char *text)
{
    DFA_State *state;
    DFA_Move *move;
    size_t i, j;
    char c;
    bool moved;

    state = dfa->begin;
    c = text[i = 0];
    while (c != '\0') {
        moved = false;
        j = 0;
        while (j < state->moves_count) {
            move = &(state->moves[j]);
            if (move->symbol == c) {
                state = move->next;
                moved = true;
                c = text[++i];
                break;
            }
            j++;
        }

        if (!moved) {
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
