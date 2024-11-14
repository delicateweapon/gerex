#include <pthread.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define MAX_ALLOCATIONS (1 << 8)
extern void *g_allocations[MAX_ALLOCATIONS];
extern size_t g_allocations_count;

void allocations_free_all(void);

extern pthread_t g_regex_thread;

struct nfa_state;
#define SYMBOL_EPSILON 0

#define MAX_SYMBOLS (1 << 4)
extern char g_symbols[MAX_SYMBOLS];
extern size_t g_symbols_count;

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
NFA_State *NFA_State_find_symbol_next(NFA_State *state, char symbol);

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


struct dfa_state;

typedef struct {
    struct dfa_state *next;
    char symbol;
} DFA_Move;

typedef struct dfa_state {
    size_t id;
    DFA_Move *moves;
    size_t moves_count;
} DFA_State;

#define MAX_DFA_STATES (1 << 6)
extern DFA_State *g_DFA_States[MAX_DFA_STATES];
extern size_t g_DFA_State_count;

DFA_State *DFA_State_create(void);
void DFA_State_move_add(DFA_State *state, DFA_State *next, char symbol);

typedef struct {
    DFA_State *begin;

    DFA_State **ends;
    size_t ends_count;
    size_t ends_capacity;
} DFA;

DFA *DFA_construct(NFA *nfa);
bool DFA_match(const char *text);

void globals_reset(void);
