/*
** EPITECH PROJECT, 2026
** minishell1
** File description:
** MiniShell translation unit
** containing commands related
** to current dir manipulation
** Author:
** Amélie Ambleton--Guth
** February 2026
*/

#include "ms_builtins.h"

int ms_bi_env(ms_shell_context_t *ctx, char **args)
{
    list_t *lst = ctx->env;
    ms_env_entry_t *entry;

    if (args && args[0]) {
        my_dprintf(2, "env: Too many arguments.\n");
        return 1;
    }
    while (lst) {
        entry = lst->data;
        my_printf("%s=%s\n", entry->key, entry->value);
        lst = lst->next;
    }
    return 0;
}
