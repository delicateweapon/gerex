#include <pthread.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

extern pthread_t g_regex_thread;

struct nfa_state;
#define SYMBOL_EPSILON 0

typedef struct {
    struct nfa_state *next;
    char symbol;
} NFA_Move;

typedef struct nfa_state {
    size_t id;
    NFA_Move *moves;
    size_t moves_count;
} NFA_State;

extern size_t g_NFA_State_count;

NFA_State *NFA_State_create(void);
void NFA_State_move_add(NFA_State *state, NFA_State *next, char symbol);

typedef struct nfa {
    NFA_State *begin;
    NFA_State *end;
} NFA;

NFA *NFA_create(bool state_init);
NFA *NFA_from_symbol(char symbol);

typedef enum {
    LPAREN,
    RPAREN,
    UNION,
    CONCAT,
    CLOSURE,
    THREE_FOURTH_CLOSURE,
} NFA_Op;

uint8_t NFA_Op_precedence(NFA_Op op);

NFA *NFA_union(NFA *nfa1, NFA *nfa2);
NFA *NFA_concat(NFA *nfa1, NFA *nfa2);
NFA *NFA_closure(NFA *nfa);
NFA *NFA_three_fourth_closure(NFA *nfa);

NFA *NFA_parse(const char *expr);
