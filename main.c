#include "regulus.h"

#include <stdio.h>

pthread_t g_regex_thread;

void *regex_func(void *args)
{
    NFA *nfa = NFA_parse(args);
    printf("Oh look, it got parsed\n");
    return NULL;
}

int main(void)
{
    char expr[64];

    while (1) {
        printf(">>> ");
        scanf("%s", expr);
        pthread_create(&g_regex_thread, NULL, regex_func, expr);
        pthread_join(g_regex_thread, NULL);
        allocations_free_all();
    }
    return 0;
}
