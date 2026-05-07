/*
** EPITECH PROJECT, 2026
** G-PSU-200-STG-2-1-42sh-1
** File description:
** ms_bi_glob
*/

#include "minishell1.h"
#include "minishell2.h"
#include "globbing.h"
#include "shell.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int ms_bi_glob(ms_shell_context_t *context, char **args)
{
    char **expanded;
    int i = 0;
    int j;

    while (args[i]) {
        expanded = glob_expand(args[i], context);
        if (!expanded || !expanded[0]) {
            i++;
            continue;
        }
        j = 0;
        while (expanded[j]) {
            write(1, expanded[j], my_strlen(expanded[j]));
            write(1, "\0", 1);
            j++;
        }
        free_str_arr(expanded);
        i++;
    }
    return 0;
}
