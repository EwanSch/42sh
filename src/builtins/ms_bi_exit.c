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

static int verify_exit_validity(char **args, int *status)
{
    *status = 0;
    if (args[0] != NULL && args[1] != NULL) {
        my_fputs(2, "exit: Expression Syntax.\n");
        return 1;
    }
    if (!args[0] || my_strlen(args[0]) == 0)
        return 0;
    if (!my_str_isnumerical(args[0])) {
        my_dprintf(2, "exit: %s.\n", my_isnumerical(args[0][0]) ?
            "Badly formed number" : "Expression Syntax");
        return 1;
    }
    *status = my_getexit(args[0]);
    return 0;
}

int ms_bi_exit(ms_shell_context_t *ctx, char **argv)
{
    int status;

    if (verify_exit_validity(argv, &status))
        return 1;
    free_str_arr(argv - 1);
    ms_teardown(ctx);
    exit(status);
}
