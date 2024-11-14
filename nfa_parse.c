#include "regulus.h"

#include <stdio.h>
#include <ctype.h>

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
            /* is any code needed for this case? no i dont think so */
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
             nfas[nfas_count++] = NFA_union(nfa2, nfa1);
             break;

         case CLOSURE:
             if (nfas_count < 1) {
                 fprintf(stderr, "parse error: Could not form closure\n");
                 pthread_exit(NULL);
             }
             nfa1 = nfas[--nfas_count];
             nfas[nfas_count++] = NFA_closure(nfa1);
             break;
        }

        if (ops_count == 0) {
            break;
        }
        top = ops[ops_count - 1];
    }
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
                fprintf(stderr, "parse_error: check for bracket placements or missing brackets\n");
                pthread_exit(NULL);
            }

            append_concat = true;
            break;

        case '\\':
            treat_as_symbol = true;
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
