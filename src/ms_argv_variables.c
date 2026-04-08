/*
** EPITECH PROJECT, 2025
** 42sh
** File description:
** ms_argv_variables.c
*/

#include <stdio.h>
#include <strings.h>
#include <unistd.h>
#include <stdlib.h>
#include "benjalib.h"
#include "minishell1.h"
#include "minishell2.h"
#include "shell.h"

static void check_flag_argv(char **argv, int *total, int *nb)
{
    if (argv[*total][0] == '-') {
        if (argv[*total][1] == 'c')
            *total += 1;
        *nb += 1;
    }
}

void save_argv(ms_shell_context_t *context, char **argv, int argc)
{
    int nb_argv = 1;
    int total = 1;
    char buffer[256];
    int nb = 0;

    for (; total < argc; total++) {
        nb = 0;
        check_flag_argv(argv, &total, &nb);
        if (nb == 1)
            continue;
        snprintf(buffer, sizeof(buffer), "argv[%d]", nb_argv);
        km_set(buffer, argv[total], &context->variables);
        nb_argv++;
    }
}
