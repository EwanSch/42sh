/*
** EPITECH PROJECT, 2026
** G-PSU-200-STG-2-1-42sh-1
** File description:
** ms_fs_glob_match
*/

#include "globbing.h"

static int handle_star(const char *p, const char *s, int i, int j)
{
    if (!p[i + 1])
        return 1;
    if (glob_match(p + i + 1, s + j))
        return 1;
    while (s[j]) {
        if (glob_match(p + i + 1, s + j + 1))
            return 1;
        j++;
    }
    return 0;
}

static int handle_bracket(const char *p, const char *s, int *i, int *j)
{
    if (!match_range(p, s[*j], i))
        return 0;
    (*i)++;
    (*j)++;
    return 1;
}

static int match_char(const char *p, const char *s, int *i, int *j)
{
    if (p[*i] == '?') {
        if (!s[*j])
            return 0;
        (*i)++;
        (*j)++;
        return 1;
    }
    if (p[*i] == '[')
        return handle_bracket(p, s, i, j);
    if (p[*i] != s[*j])
        return 0;
    (*i)++;
    (*j)++;
    return 1;
}

int glob_match(const char *p, const char *s)
{
    int i = 0;
    int j = 0;

    while (p[i]) {
        if (p[i] == '*')
            return handle_star(p, s, i, j);
        if (!match_char(p, s, &i, &j))
            return 0;
    }
    return s[j] == '\0';
}
