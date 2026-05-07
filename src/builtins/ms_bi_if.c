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

#include <string.h>

#include "minishell2.h"
#include "ms_builtins.h"
#include "../../include/ms_builtins.h"
#include "../../include/benjalib/my.h"

int ms_bi_if(ms_shell_context_t *ctx, char **args)
{
    bool err = false;
    long nbr = 0;
    char *cmd = NULL;
    int res = 0;

    nbr = ms_expression(args, &cmd, &err);
    printf("cmd: %s\n", cmd);
    if (err == false) {
        if (!cmd || *cmd == '\0') {
            dprintf(1, "if: Empty if.\n");
        }
        if (cmd && nbr > 0) {
            res = process_line(ctx, &cmd);
            free(cmd);
            return res;
        }
    } else
        res = 1;
    safe_free(&cmd);
    return res;
}
