#include "dfa.h"

#include <stdio.h>

void print_dfa(void)
{
    for (size_t i = 0; i < g_DFA_State_count; ++i) {
        DFA_State state = g_DFA_State_storage[i];
        if (state.is_end) {
            printf("%zu (end):\n", state.id);
        } else {
            printf("%zu:\n", state.id);
        }
        for (size_t j = 0; j < state.move_count; ++j) {
            printf("  %c: %zu\n", state.moves[j].symbol, state.moves[j].next->id);
        }
        printf("\n");
    }
}
