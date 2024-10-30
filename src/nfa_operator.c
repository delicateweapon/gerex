#include "nfa.h"

NFA *nfa_apply_closure(NFA *nfa) {
  State *new_start = state_create();
  State *new_end = state_create();

  state_add_move(new_start, new_end, SYMBOL_EPSILON);
  state_add_move(new_start, nfa->start, SYMBOL_EPSILON);
  state_add_move(nfa->end, nfa->start, SYMBOL_EPSILON);
  state_add_move(nfa->end, new_end, SYMBOL_EPSILON);

  nfa->start = new_start;
  nfa->end = new_end;

  return nfa;
}

NFA *nfa_concat(NFA *nfa1, NFA *nfa2) {
  NFA *result = nfa_create();

  result->start = nfa1->start;
  state_copy(nfa1->end, nfa2->start);
  nfa2->start = nfa1->end;

  return result;
}
