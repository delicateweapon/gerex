#pragma once

#include "nfa.h"

int parse_regex_to_nfa(NFA *nfa, const char *regex);
