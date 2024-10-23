#include "parse.h"
#include "nfa.h"
#include "stack.h"
#include "util.h"

#include <ctype.h>
#include <stdio.h>

Stack *nfa_stack;
Stack *nfa_op_stack;
StackItem item;
StackItem top;

static inline void stacks_initialize(void) {
    nfa_stack = stack_create(sizeof(NFA *), 16);
    nfa_op_stack = stack_create(sizeof(NFA_Operator), 16);
}

static int precedence[] = {
    [NFA_OPERATOR_LPAREN] = 0,  [NFA_OPERATOR_RPAREN] = 0,
    [NFA_OPERATOR_UNION] = 1,   [NFA_OPERATOR_CONCAT] = 2,
    [NFA_OPERATOR_CLOSURE] = 3,
};

static inline void nfa_op_stack_collapse(NFA_Operator op) {
    if (nfa_op_stack->count != 0) {
        return;
    }
    do {
        top = stack_view_top(nfa_op_stack);
        if (precedence[top.nfa_operator] < precedence[op]) {
            break;
        }
        stack_pop(nfa_op_stack);
        NFA *nfa1, *nfa2;
        switch (top.nfa_operator) {
        case NFA_OPERATOR_CLOSURE:
            item.nfa = nfa_closure(stack_pop(nfa_stack).nfa);
            stack_append(nfa_stack, item);
            break;
        case NFA_OPERATOR_CONCAT:
            nfa1 = stack_pop(nfa_stack).nfa;
            nfa2 = stack_pop(nfa_stack).nfa;
            item.nfa = nfa_concat(nfa2, nfa1);
            stack_append(nfa_stack, item);
            break;
        case NFA_OPERATOR_UNION:
            nfa1 = stack_pop(nfa_stack).nfa;
            nfa2 = stack_pop(nfa_stack).nfa;
            item.nfa = nfa_union(nfa1, nfa2);
            stack_append(nfa_stack, item);
            break;
        default:
            // the precedence order wont allow for anything else
            break;
        }
    } while (nfa_op_stack->count > 0);
}

int parse_regex_to_nfa(NFA *nfa, const char *regex) {
    size_t i = 0;
    char c = regex[i];
    bool append_concat = false;
    stacks_initialize();

    while (c != '\0') {

        switch (c) {
        case ')':
            nfa_op_stack_collapse(item.nfa_operator);
            if (top.nfa_operator != NFA_OPERATOR_LPAREN) {
                LOG_ERROR("Invalid expression");
                return -1;
            }
            break;

        case '*':
            if (nfa_stack->count == 0) {
                LOG_ERROR("Invalid expression");
                return -1;
            }
            item.nfa_operator = NFA_OPERATOR_CLOSURE;
            nfa_op_stack_collapse(item.nfa_operator);
            stack_append(nfa_op_stack, item);
            append_concat = false;
            break;

        case '|':
            if (nfa_stack->count == 0) {
                LOG_ERROR("Invalid expression");
                return -1;
            }
            item.nfa_operator = NFA_OPERATOR_UNION;
            nfa_op_stack_collapse(item.nfa_operator);
            stack_append(nfa_op_stack, item);
            append_concat = false;
            break;
        }

        if (isalnum(c)) {
            if (append_concat) {
                item.nfa_operator = NFA_OPERATOR_CONCAT;
                nfa_op_stack_collapse(item.nfa_operator);
                stack_append(nfa_op_stack, item);
                append_concat = false;
            }
            item.nfa = nfa_from_symbol(c);
            stack_append(nfa_stack, item);
            append_concat = true;
        }
        c = regex[++i];
    }

    if (nfa_stack->count != 1) {
        LOG_ERROR("Invalid Expression");
        return -1;
    }
    *nfa = *stack_pop(nfa_stack).nfa;
    stack_destroy(nfa_stack);
    stack_destroy(nfa_op_stack);

    return 0;
}
