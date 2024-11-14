#include "regulus.h"

void globals_reset(void)
{
    g_allocations_count = 0;
    g_NFA_State_count = 0;
    g_DFA_State_count = 0;
}
