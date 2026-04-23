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

int ms_bi_unsetenv(ms_shell_context_t *ctx, char **args)
{
    if (!args || !args[0]) {
        my_dprintf(2, "unsetenv: Too few arguments.\n");
        return 1;
    }
    for (int i = 0; args[i]; i++)
        km_unset(args[i], &ctx->env);
    return 0;
}
