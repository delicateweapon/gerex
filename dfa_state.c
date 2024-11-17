#include "dfa.h"

#include <stdlib.h>
#include <stdio.h>

DFA_State *g_DFA_State_storage;
size_t g_DFA_State_storage_capacity = 0;
size_t g_DFA_State_count = 0;

#define INIT_CAPACITY (1 << 6)

static bool initialized = false;

static inline int initialize(void) 
{
    g_DFA_State_storage = malloc(sizeof(DFA_State) * INIT_CAPACITY);
    if (g_DFA_State_storage == NULL) {
        fprintf(stderr, "malloc error: failed to allocate storage for DFA_State(s)\n");
        return -1;
    }
    g_DFA_State_storage_capacity = INIT_CAPACITY;
    g_DFA_State_count = 0;

    initialized = true;
    return 0;
}

DFA_State *DFA_State_create(void) 
{
    if (g_DFA_State_count == g_DFA_State_storage_capacity) {
        if (!initialized) {
            int result_code = initialize();
            if (result_code != 0) {
                return NULL;
            }
        }

        DFA_State *temp = realloc(g_DFA_State_storage, sizeof(DFA_State) * (g_DFA_State_storage_capacity * 1.5));
        if (temp == NULL) {
            fprintf(stderr, "realloc error: failed to increase storage for DFA_State(s)\n");
            return NULL;
        }

        g_DFA_State_storage = temp;
    }

    DFA_State *result;

    result = &(g_DFA_State_storage[g_DFA_State_count]);
    result->id = g_DFA_State_count;
    result->moves = NULL;
    result->move_count = 0;
    result->is_end = false;

    g_DFA_State_count++;
    return result;
}

int DFA_State_move_add(DFA_State *state, DFA_State *next, char symbol)
{
    DFA_Move *temp;

    temp = realloc(state->moves, sizeof(DFA_Move) * (state->move_count + 1));
    if (temp == NULL) {
        fprintf(stderr, "realloc error: %s\n", __func__);
        return -1;
    }

    state->moves = temp;
    state->moves[state->move_count] = (DFA_Move) {
        .next = next,
        .symbol = symbol,
    };
    state->move_count++;

    return 0;
}
