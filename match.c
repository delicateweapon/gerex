#include "match.h"

#include <stdio.h>
#include <stdlib.h>

void print_text_with_matches(const char *text, Match *matches, size_t match_count)
{
    Match m;
    size_t text_i;

    text_i = 0;
    for (size_t i = 0; i < match_count; ++i) {
        m = matches[i];

        while (text_i < m.start) {
            printf("%c", text[text_i]);
            text_i++;
        }

        printf(MATCH_BG MATCH_FG);

        while (text_i <= m.end) {
            printf("%c", text[text_i]);
            text_i++; 
        }

        printf(MATCH_RESET);
    }

    printf("%s", text + text_i);
}

Matches *Matches_create(size_t capacity)
{
    Matches *result = malloc(sizeof(*result));
    if (result == NULL) {
        fprintf(stderr, "malloc error: %s\n", __func__);
        return NULL;
    }

    result->p = malloc(sizeof(Match) * capacity);
    if (result == NULL) {
        fprintf(stderr, "malloc error: %s\n", __func__);
        return NULL;
    }

    result->count = 0;
    result->capacity = capacity;

    return result;
}

int Matches_append(Matches *matches, Match m)
{
    if (matches->count == matches->capacity) {
        Match *temp = realloc(matches->p, sizeof(Match) * (matches->capacity * 1.5));
        if (temp == NULL) {
            fprintf(stderr, "malloc error: %s\n", __func__);
            return -1;
        }
        matches->capacity *= 1.5;
        matches->p = temp;
    }

    matches->p[matches->count++] = m;   
    return 0;
}
