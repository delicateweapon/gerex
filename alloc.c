#include "regulus.h"

#include <stdlib.h>

void *g_allocations[MAX_ALLOCATIONS];
size_t g_allocations_count = 0;

void allocations_free_all(void)
{
    if (g_allocations_count == 0) {
        return;
    }
    do {
        g_allocations_count--;
        free(g_allocations[g_allocations_count]);
    } while (g_allocations_count > 0);
}
