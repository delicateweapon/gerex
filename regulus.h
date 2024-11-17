#pragma once

#ifdef _WIN64 
#define _CRT_SECURE_NO_WARNINGS
#endif

#define REGEX_MAX_LENGTH (1 << 6)
extern char g_regex[REGEX_MAX_LENGTH];
extern size_t g_regex_length;

#define TEXT_MAX_LENGTH (1 << 12)
extern char g_text[TEXT_MAX_LENGTH];
extern size_t g_text_length;
