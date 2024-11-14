#include "regulus.h"

#include <stdio.h>

pthread_t g_regex_thread;

static char text[1024];
static char expr[64];

void *regex_func(void *args);

int main(void)
{
    while (1) {
        printf(">>> ");
        scanf("%s %s", expr, text);

        pthread_create(&g_regex_thread, NULL, regex_func, NULL);
        pthread_join(g_regex_thread, NULL);

        allocations_free_all();
        globals_reset();
    }
    return 0;
}

void *regex_func(void *args)
{
    NFA *nfa = NFA_parse(expr);
    printf("Oh look, it got parsed\n");
    DFA *dfa = DFA_construct(nfa);
    bool match = DFA_match(dfa, text);
    if (match) {
        printf("it matched!\n");
    } else {
        printf("it did not match\n");
    }
    return NULL;
}
