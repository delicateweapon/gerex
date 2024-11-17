#include "nfa.h"

#include <ctype.h>
#include <stdio.h>

static NFA **nfas;
static size_t nfas_count;
static size_t nfas_capacity;

static NFA_Op **ops;
static size_t ops_count;
static size_t ops_capacity;

static NFA_Op top;
static NFA *nfa1, *nfa2, *nfa_temp;

#define INIT_CAPACITY (1 << 6)
static inline int initialize(void)
{
    if (nfas_capacity > INIT_CAPACITY) {
        return 0;
    }

    nfas = malloc(sizeof(NFA *) * INIT_CAPACITY);
    if (nfas == NULL) {
        fprintf(stderr, "malloc error: %s\n", __func__);
        return -1;
    }
    nfas_capacity = INIT_CAPACITY;

    ops = malloc(sizeof(NFA_Op) * INIT_CAPACITY);
    if (ops == NULL) {
        fprintf(stderr, "malloc error: %s\n", __func__);
        return -1;
    }
    ops_capacity = INIT_CAPACITY;

    return 0;
}

#define NFAS_APPEND(_nfa)                                                           \
    do {                                                                            \
        if (nfas_count == nfas_capacity) {                                          \
            NFA **temp = realloc(nfas, sizeof(NFA *) * (nfas_capacity * 1.5));      \
            if (temp == NULL) {                                                     \
                fprintf(stderr, "parse error: failed to increase nfas capacity\n"); \
                return NULL;                                                        \
            }                                                                       \
            nfas_capacity *= 1.5;                                                   \
        }                                                                           \
        nfas[nfas_count++] = _nfa;                                                  \
    } while (0)

#define OPS_APPEND(_op)                                                            \
    do {                                                                           \
        if (ops_count == ops_capacity) {                                           \
            NFA_Op *temp = realloc(ops, sizeof(NFA *) * (ops_capacity * 1.5));     \
            if (temp == NULL) {                                                    \
                fprintf(stderr, "parse error: failed to increase ops capacity\n"); \
                return NULL;                                                       \
            }                                                                      \
            ops_capacity *= 1.5;                                                   \
        }                                                                          \
        ops[nfas_count++] = _op;                                                   \
    } while (0)

static int ops_collapse(NFA_Op op)
{
    if (ops_count == 0) {
        return 0;
    }

    top = ops[ops_count - 1];

    while (NFA_Op_precedence(top) >= NFA_Op_precedence(op)) {
        ops_count--;

        switch (top) {
        case LPAREN:
            if (op == RPAREN) {
                return 0;
            }
            fprintf(stderr, "parse_error: unknown reason\n");
            return -1;

        case RPAREN:
            /* Yeah, i dont think any code goes here... at least for now */
            break;

        case UNION:
            if (nfas_count < 2) {
                fprintf(stderr, "parse_error: failed to collapse union (Min 2 args required)\n");
                return -1;
            }
            nfa1 = nfas[--nfas_count];
            nfa2 = nfas[--nfas_count];

            nfa_temp = NFA_union(nfa1, nfa2);
            if (nfa_temp == NULL) {
                fprintf(stderr, "parse_error: failed to collapse union\n");
                return -1;
            }

            nfas[nfas_count++] = nfa_temp;
            break;

        case CONCAT:
            if (nfas_count < 2) {
                fprintf(stderr, "parse_error: failed to collapse concat (Min 2 args required)\n");
                return -1;
            }
            nfa1 = nfas[--nfas_count];
            nfa2 = nfas[--nfas_count];

            /* yep, nfa2 goes first here, don't dare change that! */
            nfa_temp = NFA_concat(nfa2, nfa1);
            if (nfa_temp == NULL) {
                fprintf(stderr, "parse_error: failed to collapse concat\n");
                return -1;
            }

            nfas[nfas_count++] = nfa_temp;
            break;

        case CLOSURE:
            if (nfas_count < 1) {
                fprintf(stderr, "parse_error: failed to collapse closure (Min 1 args required)\n");
                return -1;
            }
            nfa1 = nfas[--nfas_count];

            nfa_temp = NFA_closure(nfa1);
            if (nfa_temp == NULL) {
                fprintf(stderr, "parse_error: failed to collapse closure\n");
                return -1;
            }

            nfas[nfas_count++] = nfa_temp;
            break;

        case THREE_FOURTH_CLOSURE:
            if (nfas_count < 1) {
                fprintf(stderr, "parse_error: failed to collapse three_fourth_closure (Min 1 args required)\n");
                return -1;
            }
            nfa1 = nfas[--nfas_count];

            nfa_temp = NFA_three_fourth_closure(nfa1);
            if (nfa_temp == NULL) {
                fprintf(stderr, "parse_error: failed to collapse three_fourth_concat\n");
                return -1;
            }

            nfas[nfas_count++] = nfa_temp;
            break;
        }

        if (ops_count == 0) {
            return 0;
        }

        top = ops[ops_count - 1];
    }

    return 0;
}

#define COLLAPSE_TILL(op)                   \
    do {                                    \
        int result_code = ops_collapse(op); \
        if (result_code != 0) {             \
            return NULL;                    \
        }                                   \
    } while (0)

NFA *NFA_parse(char *regex)
{
    if (initialize() != 0) {
        fprintf(stderr, "parse_error: failed to initialize the requierd arrays\n");
        return NULL;
    };

    ops[0] = LPAREN;
    ops_count = 1;

    NFA *result;
    size_t i;
    char c;
    bool append_concat, treat_as_alnum;

    append_concat = false;
    treat_as_alnum = false;
    c = regex[i = 0];

    while (c != '\0') {

        if (isalnum(c) || treat_as_alnum) {
            if (append_concat) {
                COLLAPSE_TILL(CONCAT);
                OPS_APPEND(CONCAT);
            }

            nfa_temp = NFA_from_symbol(c);
            if (nfa_temp == NULL) {
                return NULL;
            }
            NFAS_APPEND(NFA_from_symbol(c));

            treat_as_alnum = false;
            append_concat = true;
            c = regex[++i];
            continue;
        }

        switch (c) {
        case '*':
            COLLAPSE_TILL(CLOSURE);
            OPS_APPEND(CLOSURE);
            append_concat = true;
            break;

        case '+':
            COLLAPSE_TILL(THREE_FOURTH_CLOSURE);
            OPS_APPEND(THREE_FOURTH_CLOSURE);
            append_concat = true;
            break;

        case '|':
            COLLAPSE_TILL(UNION);
            OPS_APPEND(UNION);
            append_concat = false;
            break;

        case '(':
            if (append_concat) {
                COLLAPSE_TILL(CONCAT);
                OPS_APPEND(CONCAT);
            }

            OPS_APPEND(LPAREN);
            append_concat = false;
            break;

        case ')':
            COLLAPSE_TILL(RPAREN);
            if (ops[ops_count] != LPAREN) {
                fprintf(stderr, "parse_error: missing \'(\' for \')\' at index %zu\n", i);
                return NULL;
            }

            append_concat = true;
            break;

        case '[':
            /* TODO */
            break;

        case '\\':
            treat_as_alnum = true;
            break;

        default:
            fprintf(stderr, "parse_error: invalid character %c found at index %zu\n", c, i);
            return NULL;
        }

        c = regex[++i];
    }

    OPS_COLLAPSE(RPAREN);

    if (ops_count != 0 || nfas_count != 1) {
        fprintf(stderr, "parse_error: unknown reason, debug and you MIGHT know why, good fucking luck\n");
        return NULL;
    }

    result = nfas[--nfas_count];
    return result;
}