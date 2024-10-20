#include "parse.h"
#include "nfa.h"
#include "stack.h"
#include "util.h"

#include <ctype.h>
#include <stdio.h>

static inline void nfa_operator_collapse(Stack *nfa_operator_stack,
                                         Stack *nfa_stack) {
    StackItem s_item = stack_view_top(nfa_operator_stack);

    switch (s_item.nfa_operator) {
    case NFA_OPERATOR_LPAREN:
        break;
    case NFA_OPERATOR_CLOSURE:
        s_item.nfa = nfa_closure(stack_pop(nfa_stack).nfa);
        stack_append(nfa_stack, s_item);
        break;
    case NFA_OPERATOR_CONCAT:
        NFA *nfa1 = stack_pop(nfa_stack).nfa;
        NFA *nfa2 = stack_pop(nfa_stack).nfa;
        s_item.nfa = nfa_concat(nfa2, nfa1);
        stack_append(nfa_stack, s_item);
        break;
    case NFA_OPERATOR_UNION:
        s_item.nfa =
            nfa_union(stack_pop(nfa_stack).nfa, stack_pop(nfa_stack).nfa);
        stack_append(nfa_stack, s_item);
        break;
    }
}

NFA parse_regex_to_nfa(const char *regex) {
    size_t i;
    char c;
    StackItem s_item;
    bool should_concat;

    Stack *nfa_stack = stack_create(sizeof(NFA *), 16);
    Stack *nfa_operator_stack = stack_create(sizeof(NFA_Operator), 16);

    i = 0;
    c = regex[i];
    should_concat = false;

    while (c != '\0') {

        switch (c) {
        case '(':
            if (should_concat) {
                s_item.nfa_operator = NFA_OPERATOR_CONCAT;
                stack_append(nfa_operator_stack, s_item);
                should_concat = false;
            }
            s_item.nfa_operator = NFA_OPERATOR_LPAREN;
            stack_append(nfa_operator_stack, s_item);
            should_concat = false;
            break;
        case ')':
            should_concat = true;
            break;
        case '*':
            s_item.nfa_operator = NFA_OPERATOR_CLOSURE;
            stack_append(nfa_operator_stack, s_item);
            break;
        case '|':
            s_item.nfa_operator = NFA_OPERATOR_UNION;
            stack_append(nfa_operator_stack, s_item);
            break;
        default:
        }

        if (isalnum(c)) {
            if (should_concat) {
                s_item.nfa_operator = NFA_OPERATOR_CONCAT;
                stack_append(nfa_operator_stack, s_item);
                should_concat = false;
            }
            s_item.nfa = nfa_from_symbol(c);
            stack_append(nfa_stack, s_item);
            should_concat = true;
        }

        c = regex[++i];
    }

    nfa_operator_collapse(nfa_operator_stack, nfa_stack);
    NFA result = *stack_view_top(nfa_stack).nfa;
    stack_destroy(nfa_stack);
    stack_destroy(nfa_operator_stack);
    return result;
}
