#include "nfa.h"
#include "util.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

State *state_create(void) {
  State *state = malloc(sizeof(*state));
  if (state == NULL) {
    LOG_ERROR_HEAP_ALLOC("state");
  }
  state->eps_moves = NULL;
  state->eps_count = 0;
  memset(state->moves, 0, sizeof(state->moves));
  state->moves_count = 0;

#ifdef debug
  state->id = state_id++;
#endif

  return state;
}

void state_copy(State *dest, State *src) {
  State **temp = realloc(src->eps_moves, sizeof(State *) * src->eps_count);
  dest->eps_moves = temp;
  dest->eps_count = src->eps_count;
  memcpy(dest->moves, src->moves, sizeof(src->moves));
  dest->moves_count = src->moves_count;
}

void state_add_move(State *state, State *next_state, int symbol) {
  if (symbol != SYMBOL_EPSILON) {
    state->moves[symbol] = next_state;
    state->moves_count++;
    return;
  }

  State **temp =
      realloc(state->eps_moves, sizeof(State *) * (state->eps_count + 1));
  if (temp == NULL) {
    LOG_ERROR_HEAP_ALLOC("temp");
  }
  temp[state->eps_count] = next_state;
  state->eps_moves = temp;
  state->eps_count++;
}
