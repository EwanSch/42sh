/*
** EPITECH PROJECT, 2025
** 42sh
** File description:
** ms_special_variables.c
*/

#include <unistd.h>
#include <stdlib.h>
#include "benjalib.h"
#include "minishell1.h"
#include "minishell2.h"
#include "shell.h"

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
