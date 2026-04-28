/*
** EPITECH PROJECT, 2026
** aaa
** File description:
** aab
*/

#include <string.h>

#include "minishell1.h"
#include "ms_builtins.h"


void remove_alias(char **argv, alias_t **alias)
{
    alias_t **buf = alias;
    alias_t *tmp;

    while (*buf) {
        if (my_strcmp(argv[0], (*buf)->alias) &&
            my_strcmp(argv[0], "*"))
            buf = &(*buf)->next;
        tmp = (*buf)->next;
        free((*buf)->alias);
        free((*buf)->name);
        free(*buf);
        *buf = tmp;
        return;
    }
}

int ms_bi_unalias(ms_shell_context_t *context, char **argv)
{
    int argc = 0;

    for (char **arg = argv; arg[argc]; argc++);
    if (argc == 0) {
        my_dprintf(1, "unalias: Too few arguments.\n");
    }
    remove_alias(argv, &context->alias);
    return 0;
}
