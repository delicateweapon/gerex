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
    Stack *stop_points = stack_create(sizeof(size_t), 8);
    Stack *symbol_stack = stack_create(sizeof(char), 16);

    i = 0;
    c = regex[i];

    while (c != '\0') {
        switch (c) {
        case '(':
            s_item.stop_point = nfa_stack->count;
            stack_append(stop_points, s_item);
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
