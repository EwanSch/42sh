/*
** EPITECH PROJECT, 2026
** G-PSU-200-STG-2-1-42sh-1
** File description:
** ms_fs_glob_range_match
*/

#include "globbing.h"

static int is_range(const char *p, int i)
{
    return p[i + 1] == '-' && p[i + 2] != ']';
}

static void update_match(const char *p, char c,
    int *i, int *match)
{
    if (is_range(p, *i)) {
        if (c >= p[*i] && c <= p[*i + 2])
            *match = 1;
        *i += 3;
    } else {
        if (c == p[*i])
            *match = 1;
        (*i)++;
    }
}

int match_range(const char *pattern, char c, int *i)
{
    int neg = 0;
    int match = 0;

    (*i)++;
    if (pattern[*i] == '^') {
        neg = 1;
        (*i)++;
    }
    while (pattern[*i] && pattern[*i] != ']')
        update_match(pattern, c, i, &match);
    while (pattern[*i] && pattern[*i] != ']')
        (*i)++;
    return neg ? !match : match;
}
