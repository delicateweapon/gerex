#pragma once

#include <stddef.h>

typedef struct {
    size_t start;
    size_t end;
} Match;

typedef struct {
    size_t capacity;
    size_t count;
    Match *p;
} Matches;

Matches *Matches_create(size_t capacity);
int Matches_append(Matches *matches, Match m);
Matches *Matches_generate(char *regex, char *text);

#define MATCH_BG "\x1b[40m" /* Black */
#define MATCH_FG "\x1b[33m" /* Yellow */
#define MATCH_RESET "\x1b[0m" /* Reset */

void print_text_with_matches(const char *text, Match *matches, size_t match_count);
