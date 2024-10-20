#include "parse.h"
#include "stack.h"
#include "util.h"

#include <stdio.h>

int parse_regex_to_nfa(const char *regex) {
    size_t i;
    char c;

    i = 0;
    c = regex[i];

    while (c != '\0') {
        c = regex[++i];
    }
    return -1;
}
