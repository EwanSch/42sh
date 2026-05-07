/*
** EPITECH PROJECT, 2026
** G-PSU-200-STG-2-1-42sh-1
** File description:
** ms_bi_echo
*/

#include "minishell1.h"
#include "minishell2.h"
#include "shell.h"
#include "globbing.h"

static int is_n_flag(char *arg)
{
    if (!arg)
        return 0;
    return my_strcmp(arg, "-n") == 0;
}

static void print_words(char **args, int start)
{
    int i = start;

    while (args[i]) {
        write(1, args[i], my_strlen(args[i]));
        if (args[i + 1])
            write(1, " ", 1);
        i++;
    }
}

int ms_bi_echo(ms_shell_context_t *context, char **args)
{
    int newline = 1;
    int start = 0;

    (void)context;
    if (is_n_flag(args[0])) {
        newline = 0;
        start = 1;
    }
    print_words(args, start);
    if (newline)
        write(1, "\n", 1);
    return 0;
}
