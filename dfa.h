#pragma once

#include "nfa.h"

struct dfa_state;

typedef struct {
    struct dfa_state *next;
    char symbol;
} DFA_Move;

typedef struct dfa_state {
    size_t id;
    DFA_Move *moves;
    size_t move_count;
    bool is_end;
} DFA_State;

extern DFA_State *g_DFA_State_storage;
extern size_t g_DFA_State_storage_capacity;
extern size_t g_DFA_State_count;

DFA_State *DFA_State_create(void);
int DFA_State_move_add(DFA_State *state, DFA_State *next, char symbol);

extern DFA_State *g_start_state;
int DFA_construct(NFA *nfa);