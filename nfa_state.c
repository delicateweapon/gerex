#include "nfa.h"

#include <stdio.h>
#include <stdlib.h>

NFA_State *g_NFA_State_storage;
size_t g_NFA_State_storage_capacity = 0;
size_t g_NFA_State_count = 0;

bool initialized = false;

#define INIT_CAPACITY (1 << 8)

static inline int initialize(void) 
{
    g_NFA_State_storage = malloc(sizeof(NFA_State) * INIT_CAPACITY);
    if (g_NFA_State_storage == NULL) {
        fprintf(stderr, "malloc error: failed to allocate storage for NFA_State(s)\n");
        return -1;
    }
    g_NFA_State_storage_capacity = INIT_CAPACITY;
    g_NFA_State_count = 0;

    initialized = true;
    return 0;
}

NFA_State *NFA_State_create(void)
{
    if (g_NFA_State_count == g_NFA_State_storage_capacity) {
        if (!initialized) {
            int result_code = initialize();
            if (result_code != 0) {
                return NULL;
            }
        }

        NFA_State *temp = realloc(g_NFA_State_storage, sizeof(NFA_State) * (g_NFA_State_storage_capacity * 1.5));
        if (temp == NULL) {
            fprintf(stderr, "realloc error: failed to increase storage for NFA_State(s)\n");
            return NULL;
        }

        g_NFA_State_storage = temp;
    }


    NFA_State *result;

    result = &(g_NFA_State_storage[g_NFA_State_count]);

    result->id = g_NFA_State_count;
    result->moves = NULL;
    result->move_count = 0;

    g_NFA_State_count++;
    return result;
}

int NFA_State_move_add(NFA_State *state, NFA_State *next, char symbol)
{
    NFA_Move *temp;

    temp = realloc(state->moves, sizeof(NFA_Move) * (state->move_count + 1));
    if (temp == NULL) {
        fprintf(stderr, "realloc error: %s\n", __func__);
        return -1;
    }

    state->moves = temp;
    state->moves[state->move_count] = (NFA_Move) {
        .next = next,
        .symbol = symbol,
    };
    state->move_count++;

    return 0;
}

NFA_State *NFA_State_find_symbol_next(NFA_State *state, char symbol) 
{
    NFA_Move *m;
    NFA_State *result;

    if (state->move_count == 0) {
        return NULL;
    }

    if (state->move_count == 1) {
        m = &(state->moves[0]);
        if (m->symbol == symbol) {
            return m->next;
        }

        if (m->symbol != SYMBOL_EPSILON) {
            return NULL;
        }
    }

    for (size_t i = 0; i < state->move_count; ++i) {
        m = &(state->moves[i]); 
        result = NFA_State_find_symbol_next(m->next, symbol);
        if (result != NULL) {
            return result;
        }
    }

    return NULL;
}

bool NFA_State_check_end(NFA *nfa, NFA_State *state) 
{
    NFA_Move *m;
    if (state->move_count == 1) {
        m = &(state->moves[0]);
        if (m->symbol != SYMBOL_EPSILON) {
            return false;
        }
    }

    for (size_t i = 0; i < state->move_count; ++i) {
        m = &(state->moves[0]);
        if (m->next == nfa->end) {
            return true;
        }

        bool recurse = NFA_State_check_end(nfa, m->next);
        if (recurse) {
            return true;
        }
    } 

    return false;
}
