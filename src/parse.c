#include "parse.h"
#include "nfa.h"
#include "stack.h"
#include "util.h"

#include <ctype.h>
#include <stdio.h>

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
        if (should_concat) {
            s_item.nfa_operator = NFA_OPERATOR_CONCAT;
            stack_append(nfa_operator_stack, s_item);
            should_concat = false;
        }

        switch (c) {
        case '(':
            s_item.nfa_operator = NFA_OPERATOR_LPAREN;
            stack_append(nfa_operator_stack, s_item);
            should_concat = false;
            break;
        case ')':
            should_concat = true;
            break;
        default:
        }

        if (isalnum(c)) {
            s_item.nfa = nfa_from_symbol(c);
            stack_append(nfa_stack, s_item);
            should_concat = true;
        }

        c = regex[++i];
    }

    NFA result = *stack_view_top(nfa_stack).nfa;
    stack_destroy(nfa_stack);
    stack_destroy(nfa_operator_stack);
    return result;
}
