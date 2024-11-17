#include "dfa.h"
#include "match.h"
#include "time.h"

bool DFA_match(char *text)
{
    DFA_Move *move;
    DFA_State *state;
    size_t i, j;
    char c;
    bool moved;

    state = g_start_state;
    c = text[i = 0];
    while (c != '\0') {
        moved = false;
        j = 0;
        while (j < state->move_count) {
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

    return state->is_end;
}

Matches *Matches_generate(char *regex, char *text)
{
    Matches *matches = Matches_create((1 << 6));
    if (matches == NULL) {
        return NULL;
    }

    NFA *nfa = NFA_parse(regex);
    if (nfa == NULL) {
        return NULL;
    }

    if (DFA_construct(nfa) != 0) {
        return NULL;
    }

    DFA_Move *move;
    DFA_State *state;
    size_t i, j, begin, end;
    char c;
    bool moved;

    state = g_start_state;
    begin = 0;
    end = 0;
    c = text[i = 0];

    while (c != '\0') {
        moved = false;
        j = 0;
        while (j < state->move_count) {
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
            if (end >= begin) {
                Matches_append(matches, (Match) { begin, end });
            }
            state = g_start_state;

            bool update_i = true;
            for (size_t k = 0; k < state->move_count; ++k) {
                if (state->moves[k].symbol == c) {
                    update_i = false;
                    break;
                }
            }

            if (update_i) {
                begin = i + 1;
                c = text[++i];
            }
        } else {
            if (state->is_end) {
                if (i != 0) {
                    end = i - 1;
                }
            }
        }
    }

    if (end > begin) {
        if (state->is_end) {
            Matches_append(matches, (Match) { begin, end });
        }
    }

    return matches;
}
