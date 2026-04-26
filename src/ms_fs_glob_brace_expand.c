/*
** EPITECH PROJECT, 2026
** G-PSU-200-STG-2-1-42sh-1
** File description:
** ms_fs_glob_brace_expand
*/

#include "globbing.h"

static char *build_pattern(const char *pattern,
    char *open, char *close, char *part)
{
    char *res = my_calloc(1024, 1);

    strncat(res, pattern, open - pattern);
    strcat(res, part);
    strcat(res, close + 1);
    return res;
}

static int count_parts(char **parts)
{
    int i = 0;

    while (parts[i])
        i++;
    return i;
}

char **brace_expand(const char *pattern)
{
    char *open = strchr(pattern, '{');
    char *close;
    char *inside;
    char **parts;
    char **res;
    int count;

    if (!open || !(close = strchr(open, '}')))
        return NULL;
    inside = my_strndup(open + 1, close - open - 1);
    parts = my_explode(inside, ",");
    count = count_parts(parts);
    res = malloc(sizeof(char *) * (count + 1));
    for (int i = 0; i < count; i++)
        res[i] = build_pattern(pattern, open, close, parts[i]);
    res[count] = NULL;
    free(inside);
    free_str_arr(parts);
    return res;
}
