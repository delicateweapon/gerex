#include "match.h"
#include "regulus.h"
#include "help.h"

#include <stdio.h>
#include <string.h>

char g_regex[REGEX_MAX_LENGTH];
size_t g_regex_length;

char g_text[TEXT_MAX_LENGTH];
size_t g_text_length;

int main(int argc, char **argv)
{
    if (argc != 3) {
        fprintf(stderr, "error: Minimum two args required\n");
        printf("%s", g_help_message);
        return -1;
    }

    g_regex_length = sprintf(g_regex, "%s", argv[1]);
    const char *file_path = argv[2];

    FILE *f = fopen(file_path, "rb");
    if (f == NULL) {
        fprintf(stderr, "error: failed to open file: %s\n", file_path);
        return -1;
    }

    g_text_length = fread(g_text, sizeof(char), TEXT_MAX_LENGTH, f);
    fclose(f);

    Match matches[3] = {
        {3, 13},
        {45, 80},
        {120, 130},
    };

    print_text_with_matches(g_text, matches, 3);

    return 0;
}