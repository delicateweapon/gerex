#include "regulus.h"

#include <stdlib.h>

void *allocations[MAX_ALLOCATIONS];
size_t allocations_count;

void allocations_free_all(void)
{
    if (allocations_count == 0) {
        return;
    }
    do {
        allocations_count--;
        free(allocations[allocations_count]);
    } while (allocations_count > 0);
}
