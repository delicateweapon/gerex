#pragma once

#include "util_color.h"

#define MIN(a, b) (a) < (b) ? (a) : (b)
#define MAX(a, b) (a) > (b) ? (a) : (b)
#define CLAMP(v, a, b) ((v) < (a)) ? (a) : (((v) > (b)) ? (b) : (v))

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))
#define BOOL_TO_STR(v) (v ? "true" : "false")

#define LOG_ERROR(message)                                                     \
    fprintf(stderr, "[" ANSI_FG_RED "ERROR" ANSI_RESET "] %s (%s)\n", message, \
            __func__);

#define LOG_ERROR_HEAP_ALLOC(object_name)                                      \
    fprintf(stderr,                                                            \
            "[" ANSI_FG_RED "ERROR" ANSI_RESET                                 \
            "] Failed to allocate memory for %s (%s)\n",                       \
            object_name, __func__);
