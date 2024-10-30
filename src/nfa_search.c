#include "nfa.h"

State *state_search(State *state, int symbol) {
  State *result;

  result = state->moves[symbol];
  if (result != NULL) {
    return result;
  }

  for (size_t i = 0; i < state->eps_count; ++i) {
    result = state_search(state->eps_moves[i], symbol);
    if (result != NULL) {
      return result;
    }
  }
  
  return NULL;
}
