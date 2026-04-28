/*
** EPITECH PROJECT, 2025
** 42sh
** File description:
** ms_new_builtin.c
*/

#include "minishell1.h"
#include "minishell2.h"
#include "shell.h"
#include <string.h>
#include <stdio.h>

int is_builtin(char *str)
{
    return (!strcmp(str, "cd") || !strcmp(str, "exit") || !strcmp(str, "env")
        || !strcmp(str, "setenv") || !strcmp(str, "unsetenv") ||
        !strcmp(str, "which") || !strcmp(str, "where") ||
        !strcmp(str, "while") || !strcmp(str, "watchlog") ||
        !strcmp(str, "warp") || !strcmp(str, "wait") || !strcmp(str, "ver") ||
        !strcmp(str, "unset") || !strcmp(str, "unlimit") ||
        !strcmp(str, "universe") || !strcmp(str, "unhash") ||
        !strcmp(str, "uncomplete") || !strcmp(str, "unalias") ||
        !strcmp(str, "umask") || !strcmp(str, "time") ||
        !strcmp(str, "termname") || !strcmp(str, "telltc") ||
        !strcmp(str, "switch") || !strcmp(str, "suspend") ||
        !strcmp(str, "stop") || !strcmp(str, "source") ||
        !strcmp(str, "shift") || !strcmp(str, "setxvers") ||
        !strcmp(str, "setty"));
}

int run_which(char **args, ms_shell_context_t *context)
{
    for (int i = 1; args[i]; ++i) {
        if (is_builtin(args[i])) {
            dprintf(2, "%s: shell built-in command.\n", args[i]);
            continue;
        }
    }
    return 0;
}

int run_where(char **args, ms_shell_context_t *context)
{
    return 0;
}
