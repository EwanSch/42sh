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

static int ms_valid_envname(char *key)
{
    if (!my_isalpha(key[0]) && key[0] != '_')
        return error("setenv: Variable name must begin with a letter.\n");
    for (int i = 0; key[i]; i++)
        if ((key[i] < 'A' || key[i] > 'Z') &&
            (key[i] < 'a' || key[i] > 'z') &&
            (key[i] < '0' || key[i] > '9') &&
            key[i] != '_' && key[i] != '.')
            return error("setenv: Variable name must contain "
                "alphanumeric characters.\n");
    return 0;
}

int ms_bi_setenv(ms_shell_context_t *ctx, char **args)
{
    int argc = 0;

    for (char **arg = args; arg[argc]; argc++);
    if (argc > 2) {
        my_dprintf(2, "setenv: Too many arguments.\n");
        return 1;
    }
    if (argc == 0) {
        ms_bi_env(ctx, NULL);
        return 0;
    }
    if (ms_valid_envname(args[0]))
        return 1;
    if (args[1])
        km_set(args[0], args[1], &ctx->env);
    else
        km_set(args[0], "", &ctx->env);
    return 0;
}
