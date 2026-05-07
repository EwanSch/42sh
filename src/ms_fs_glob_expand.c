/*
** EPITECH PROJECT, 2026
** G-PSU-200-STG-2-1-42sh-1
** File description:
** ms_fs_glob_expand
*/

#include "globbing.h"

char **list_to_array(list_t *lst)
{
    int size = ll_size(lst);
    char **res = malloc(sizeof(char *) * (size + 1));
    int i = 0;

    if (!res)
        return NULL;
    while (i < size) {
        res[i] = ll_shift(&lst);
        i++;
    }
    res[i] = NULL;
    return res;
}

static void append_matches(list_t **lst, char **res)
{
    int j = 0;

    if (!res)
        return;
    while (res[j]) {
        ll_push(lst, my_strdup(res[j]));
        j++;
    }
}

static char **expand_braces(char **brace)
{
    list_t *lst = NULL;
    char **res;
    int i = 0;

    while (brace[i]) {
        res = read_dir_match(brace[i]);
        append_matches(&lst, res);
        if (res)
            free_str_arr(res);
        i++;
    }
    free_str_arr(brace);
    return list_to_array(lst);
}

char **glob_expand(char *pattern, ms_shell_context_t *ctx)
{
    char **brace = brace_expand(pattern);

    (void)ctx;
    if (brace)
        return expand_braces(brace);
    return read_dir_match(pattern);
}
