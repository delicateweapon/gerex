#include "parse.h"
#include "stack.h"
#include "util.h"

#include <ctype.h>
#include <stdio.h>

NFA *parse_regex_to_nfa(const char *regex) {
    size_t i;
    char c;
    StackItem s_item;

    Stack *nfa_stack = stack_create(sizeof(NFA *), 16);
    Stack *symbol_stack = stack_create(sizeof(char), 16);

    i = 0;
    c = regex[i];

    while (c != '\0') {
        switch (c) {
        case '(':
            break;
        case ')':
            break;
        default:
        }

        if (isalnum(c)) {
            s_item.symbol = c;
            stack_append(symbol_stack, s_item);
        }

        c = regex[++i];
    }
    return stack_view_top(nfa_stack).nfa;
}
