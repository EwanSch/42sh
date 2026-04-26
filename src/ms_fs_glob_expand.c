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

    if (!res)
        return NULL;
    for (int i = 0; i < size; i++)
        res[i] = ll_shift(&lst);
    res[size] = NULL;
    return res;
}

static void append_matches(list_t **lst, char **res)
{
    for (int j = 0; res && res[j]; j++)
        ll_push(lst, res[j]);
}

static char **expand_braces(char **brace)
{
    list_t *lst = NULL;
    char **res;

    for (int i = 0; brace[i]; i++) {
        res = read_dir_match(brace[i]);
        append_matches(&lst, res);
        free(res);
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
