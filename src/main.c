#include "nfa.h"
#include "util.h"

#include <stdbool.h>
#include <stdio.h>

struct test_case {
    const char *text;
    bool match;
};

int main(void) {
    // regex: ab*c
    NFA *nfa = nfa_concat_multiple(3, nfa_from_symbol('a'),
                                   nfa_closure(nfa_from_symbol('b')),
                                   nfa_from_symbol('c'));
    const struct test_case test_cases[] = {
        {"abc", true},   {"ac", true},     {"abbbbc", true},
        {"bbbc", false}, {"abbbb", false}, {"abcd", false}};

    size_t test_count = ARRAY_SIZE(test_cases);
    bool expected;
    bool actual;
    struct test_case test;

    for (size_t i = 0; i < test_count; ++i) {
        test = test_cases[i];
        expected = test.match;
        actual = nfa_match(nfa, test.text);

        printf("%s\texpected: %s\tactual %s\t", test.text,
               BOOL_TO_STR(expected), BOOL_TO_STR(actual));
        if (actual == expected) {
            printf(ANSI_FG_GREEN "PASSED" ANSI_RESET "\n");
        } else {
            printf(ANSI_FG_RED "FAILED" ANSI_RESET "\n");
        }
    }

    return 0;
}
