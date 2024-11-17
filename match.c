#include "match.h"

#include <stdio.h>

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