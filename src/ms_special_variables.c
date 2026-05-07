/*
** EPITECH PROJECT, 2025
** 42sh
** File description:
** ms_special_variables.c
*/

#include <strings.h>
#include <unistd.h>
#include <stdlib.h>
#include "benjalib.h"
#include "minishell1.h"
#include "minishell2.h"
#include "shell.h"

bool km_match(char *key, char *value, keymap_t *keymap)
{
    bool status = false;

    if (my_strcmp(km_get(key, keymap), value) == 0)
        status = true;
    return status;
}

void set_cwd_variable(ms_shell_context_t *context)
{
    char *cwd = getcwd(NULL, 0);

    km_set(MS_VAR_CWD, cwd, &context->variables);
    safe_free(&cwd);
}

void set_term_variable(ms_shell_context_t *context)
{
    char *term = km_get_or_default(MS_ENV_TERM, context->env, NULL);

    km_set(MS_VAR_TERM, term, &context->variables);
}

int command_eof(char c, ms_shell_context_t *context)
{
    if (c != 0x04)
        return 0;
    if (!km_match(MS_VAR_IGNOREEOF, "0", context->variables))
        return 1;
    else
        error("\n%s\n", ERROR_IGNOREEOF_CTRL_D);
    return 0;
}
