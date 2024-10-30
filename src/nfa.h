#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define debug

#ifdef debug
extern size_t state_id;
#endif

typedef struct state {
  struct state **eps_moves;
  size_t eps_count;
  struct state *moves[128];
  size_t moves_count;

#ifdef debug
  size_t id;
#endif

} State;

#define SYMBOL_EPSILON 0

State *state_create(void);
void state_add_move(State *state, State *next_state, int symbol);
void state_copy(State *dest, State *src);
State *state_search(State *state, int symbol);

typedef struct nfa {
  State *start;
  State *end;
} NFA;

NFA *nfa_create(void);
NFA *nfa_from_symbol(int symbol);

NFA *nfa_apply_closure(NFA *nfa);
NFA *nfa_concat(NFA *nfa1, NFA *nfa2);
bool nfa_match(NFA *nfa, const char *text);

#ifdef debug
void nfa_debug_print(NFA *nfa);
#endif
