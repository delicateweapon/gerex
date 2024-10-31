#include "parse.h"
#include "operator.h"
#include "stack.h"

static Stack *nfas;
static Stack *ops;

static int precedence[] = {
    [LPAREN] = 0,     [RPAREN] = 0,      [NFA_UNION] = 1,
    [NFA_CONCAT] = 2, [NFA_CLOSURE] = 3,
};

static inline void init_stacks(void) {
  nfas = stack_create(sizeof(NFA *), 8);
  ops = stack_create(sizeof(op), 8);
}

static NFA *nfa_top;
static op op_top;
static inline void op_collapse(op o) {
  op_top = stack_view_top(ops).op;
  while (precedence[o] < precedence[op_top]) {
    (void)stack_pop(ops);

    switch (op_top) {
    case LPAREN:
      break;
    case RPAREN:
      break;
    case NFA_CONCAT:
      break;
    case NFA_CLOSURE:
      break;
    case NFA_UNION:
      break;
    default:
      break;
    }

    op_top = stack_view_top(ops).op;
  }
}

void nfa_parse(NFA *nfa, const char *text) {
  init_stacks();

  size_t i = 0;
  char c = text[i];

  while (c != '\0') {
    c = text[++i];
  }
}
