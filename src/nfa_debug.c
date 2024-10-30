#include "nfa.h"

#ifdef debug

#include <stdio.h>
#include <string.h>

bool *printed_id;
size_t n;
State *maybe;

static inline void state_debug_print(State *state) {
  if (printed_id[state->id] == true) {
    return;
  }

  for (size_t i = 0; i < state->eps_count; ++i) {
    printf("%zu -> %zu\t", state->id, state->eps_moves[i]->id);
  }
  if (state->eps_count > 0) {
    printf("\n");
  }

  n = 0;
  for (size_t i = 0; i < 128; ++i) {
    maybe = state->moves[i];
    if (maybe == NULL) {
      continue;
    }

    printf("%zu -> %zu (%c)\t", state->id, maybe->id, (char)i);
    n++;
    if (n == state->moves_count) {
      break;
    }
  }
  if (state->moves_count > 0) {
    printf("\n");
  }

  printed_id[state->id] = true;
}

static inline void state_debug_print_recurse(State *state) {
  state_debug_print(state);
  for (size_t i = 0; i < state->eps_count; ++i) {
    state_debug_print_recurse(state->eps_moves[i]);
  }

  n = 0;
  for (size_t i = 0; i < 128; ++i) {
    maybe = state->moves[i];
    if (maybe == NULL) {
      continue;
    }

    state_debug_print_recurse(maybe);
    n++;
    if (n == state->moves_count) {
      break;
    }
  }
}

void nfa_debug_print(NFA *nfa) {
  bool printed_id_runtime[state_id];
  memset(printed_id_runtime, false, sizeof(printed_id_runtime));

  printed_id = printed_id_runtime;
  state_debug_print_recurse(nfa->start);
}

#endif
