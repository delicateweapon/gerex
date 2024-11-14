#include "regulus.h"

#include <ctype.h>
#include <stdio.h>

#define EPSILON_MOVE(s1, s2) \
    NFA_State_move_add(s1, s2, SYMBOL_EPSILON)

#define CAPACITY (1 << 5)

static NFA *nfas[CAPACITY];
size_t nfas_count;

static NFA_Op ops[CAPACITY];
size_t ops_count;

static NFA_Op top;
static NFA *nfa1, *nfa2;

static void ops_collapse(NFA_Op op)
{
    if (ops_count == 0) {
        return;
    }

    top = ops[ops_count - 1];
    while (NFA_Op_precedence(top) >= NFA_Op_precedence(op)) {
        ops_count--;

        switch (top) {
        case LPAREN:
        case RPAREN:
            /* is there any code needed for this case? no, i dont think so */
            break;

        case UNION:
            if (nfas_count < 2) {
                fprintf(stderr, "parse error: Could not form union\n");
                pthread_exit(NULL);
            }
            nfa1 = nfas[--nfas_count];
            nfa2 = nfas[--nfas_count];
            nfas[nfas_count++] = NFA_union(nfa1, nfa2);
            break;

        case CONCAT:
            if (nfas_count < 2) {
                fprintf(stderr, "parse error: Could not form concat\n");
                pthread_exit(NULL);
            }
            nfa1 = nfas[--nfas_count];
            nfa2 = nfas[--nfas_count];
            nfas[nfas_count++] = NFA_concat(nfa2, nfa1);
            break;

        case CLOSURE:
            if (nfas_count < 1) {
                fprintf(stderr, "parse error: Could not form closure\n");
                pthread_exit(NULL);
            }
            nfa1 = nfas[--nfas_count];
            nfas[nfas_count++] = NFA_closure(nfa1);
            break;

        case THREE_FOURTH_CLOSURE:
            if (nfas_count < 1) {
                fprintf(stderr, "parse error: Could not form three-fourth-closure\n");
                pthread_exit(NULL);
            }
            nfa1 = nfas[--nfas_count];
            nfas[nfas_count++] = NFA_three_fourth_closure(nfa1);
            break;
        }

        if (ops_count == 0) {
            break;
        }
        top = ops[ops_count - 1];
    }
}

static size_t marker;

static void form_union_till_marker(void)
{
    if (nfas_count == marker) {
        return;
    }

    NFA *nfa = NFA_create(true);

    size_t i = nfas_count;
    do {
        i--;
        EPSILON_MOVE(nfa->begin, nfas[i]->begin);
        EPSILON_MOVE(nfas[i]->end, nfa->end);
    } while (i > marker);

    nfas_count = marker;
    nfas[nfas_count++] = nfa;
}

NFA *NFA_parse(const char *expr)
{
    nfas_count = 0;
    ops[0] = LPAREN;
    ops_count = 1;

    NFA *result;
    size_t i;
    char c;
    bool append_concat;
    bool treat_as_symbol;

    append_concat = false;
    treat_as_symbol = false;
    c = expr[i = 0];
    while (c != '\0') {
        if (isalnum(c)) {
            if (append_concat) {
                ops_collapse(CONCAT);
                ops[ops_count++] = CONCAT;
            }
            nfas[nfas_count++] = NFA_from_symbol(c);

            append_concat = true;
            c = expr[++i];
            continue;
        }

        if (treat_as_symbol) {
            if (append_concat) {
                ops_collapse(CONCAT);
                ops[ops_count++] = CONCAT;
            }
            nfas[nfas_count++] = NFA_from_symbol(c);

            treat_as_symbol = false;
            append_concat = true;
            c = expr[++i];
            continue;
        }

        switch (c) {
        case '*':
            ops_collapse(CLOSURE);
            ops[ops_count++] = CLOSURE;

            append_concat = true;
            break;

        case '+':
            ops_collapse(THREE_FOURTH_CLOSURE);
            ops[ops_count++] = THREE_FOURTH_CLOSURE;

            append_concat = true;
            break;

        case '|':
            ops_collapse(UNION);
            ops[ops_count++] = UNION;

            append_concat = false;
            break;

        case '(':
            if (append_concat) {
                ops_collapse(CONCAT);
                ops[ops_count++] = CONCAT;
            }
            ops_collapse(LPAREN);
            ops[ops_count++] = LPAREN;

            append_concat = false;
            break;

        case ')':
            ops_collapse(RPAREN);
            if (ops[ops_count] != LPAREN) {
                fprintf(stderr, "parse error: check for bracket placements or missing brackets\n");
                pthread_exit(NULL);
            }

            append_concat = true;
            break;

        case '\\':
            treat_as_symbol = true;
            break;

        case '[':
            c = expr[++i];
            marker = nfas_count;
            treat_as_symbol = false;

            while (isalnum(c) || c == '\\' || treat_as_symbol) {
                if (c == '\\') {
                    if (!treat_as_symbol) {
                        treat_as_symbol = true;
                        continue;
                    }
                }

                nfas[nfas_count++] = NFA_from_symbol(c);
                c = expr[++i];

                if (treat_as_symbol) {
                    treat_as_symbol = false;
                }
            }

            if (c != ']') {
                fprintf(stderr, "parse error: ']' is missing\n");
                pthread_exit(NULL);
            }
            form_union_till_marker();

            append_concat = true;
            break;

        default:
            fprintf(stderr, "parse error: Invalid char '%c' found, use '\\%c' to treat it literally\n", c, c);
            pthread_exit(NULL);
            break;
        }

        c = expr[++i];
    }

    ops_collapse(RPAREN);
    if (ops_count != 0 || nfas_count != 1) {
        fprintf(stderr, "parse error: ummmmm... run the debugger to know why\n");
        pthread_exit(NULL);
    }

    result = nfas[0];
    return result;
}
