#include "nfa.h"
#include "util.h"

#include <stdio.h>

#ifdef debug
size_t state_id = 0;
#endif

int main(void) {
  const char *text = "abbbc";

  NFA *nfa = nfa_concat(
      nfa_concat(nfa_from_symbol('a'), nfa_apply_closure(nfa_from_symbol('b'))),
      nfa_from_symbol('c'));

  // nfa_debug_print(nfa);

  printf("%s\n", BOOL_TO_STR(nfa_match(nfa, text)));

  return 0;
}
