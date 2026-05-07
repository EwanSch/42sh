/*
** EPITECH PROJECT, 2026
** minishell1
** File description:
** MiniShell utils
** Author:
** Amélie Ambleton--Guth
*/

#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>

#include "minishell2.h"
#include "benjalib/my.h"
#include "benjalib/my_printf.h"

static int expand_prompt_sequence(char sequence, ms_shell_context_t *context)
{
    char *buf;

    switch (sequence) {
        case '/':
            buf = getcwd(NULL, 0);
            my_printf("%s", buf);
            if (buf)
                free(buf);
            return 0;
        case '?':
            my_printf("%d", context->last_exit_status);
            return 0;
        default:
            my_printf("%c", sequence);
            return 0;
    }
}

static int ms_write_prompt(char const *prompt, ms_shell_context_t *context)
{
    bool percent = false;

    for (int i = 0; prompt[i] != '\0'; i++) {
        if (prompt[i] == '%' && !percent) {
            percent = true;
            continue;
        }
        if (percent) {
            percent = false;
            expand_prompt_sequence(prompt[i], context);
            continue;
        }
        my_putchar(prompt[i]);
    }
    return 0;
}

int ms_prompt(ms_shell_context_t *context, char *type)
{
    char const *prompt;

    if (!context)
        return 1;
    if (!context->is_interactive)
        return 0;
    prompt = km_get(type, context->variables);
    if (!prompt)
        return 1;
    my_putchar('\r');
    ms_write_prompt(prompt, context);
    return 0;
}
