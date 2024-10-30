#include "nfa.h"

bool nfa_match(NFA *nfa, const char *text) {
  State *state = nfa->start;
  size_t i = 0;
  char c = text[i];

  while (c != '\0') {
    state = state_search(state, c);
    if (state == NULL) {
      return false;
    }

    c = text[++i];

    if (state == nfa->end) {
      if (c != '\0') {
        return false;
      }
      return true;
    }
  }

  return false;
}
