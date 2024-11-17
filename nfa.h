#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#define SYMBOL_EPSILON 0

struct nfa_state;

typedef struct {
    struct nfa_state *next;
    char symbol;
} NFA_Move;

typedef struct nfa_state {
    size_t id;
    NFA_Move *moves;
    size_t move_count;
} NFA_State;

extern NFA_State *g_NFA_State_storage;
extern size_t g_NFA_State_storage_capacity;
extern size_t g_NFA_State_count;

NFA_State *NFA_State_create(void);
int NFA_State_move_add(NFA_State *state, NFA_State *next, char symbol);

typedef struct {
    NFA_State *begin;
    NFA_State *end;
} NFA;

NFA *NFA_create(bool states_init);

extern char *g_symbols;
extern size_t g_symbols_capacity;
extern size_t g_symbol_count;

NFA *NFA_from_symbol(char symbol);

NFA *NFA_closure(NFA *nfa);
NFA *NFA_three_fourth_closure(NFA *nfa);
NFA *NFA_union(NFA *nfa1, NFA *nfa2);
NFA *NFA_concat(NFA *nfa1, NFA *nfa2);

typedef enum {
    LPAREN,
    RPAREN,
    UNION,
    CONCAT,
    CLOSURE,
    THREE_FOURTH_CLOSURE
} NFA_Op;

uint8_t NFA_Op_precedence(NFA_Op op);

NFA_State *NFA_State_find_symbol_next(NFA_State *state, char symbol);

NFA *NFA_parse(const char *regex);
