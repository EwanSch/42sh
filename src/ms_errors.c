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

static char *const interactive_errs[] = {
    "Ambiguous output redirect.", // MSE_AMBIGUOUS_OUTPUT
    "Ambiguous input redirect.", // MSE_AMBIGUOUS_INPUT
    "Missing name for redirect.", // MSE_MISSING_NAME_REDIRECT
    "Invalid null command.", // MSE_NULL_COMMAND
    "Command not found.", // MSE_COMMAND_NOT_FOUND
};
static char *const script_errs[] = {
    "Ambiguous output redirect.", // MSE_AMBIGUOUS_OUTPUT
    "Ambiguous input redirect.", // MSE_AMBIGUOUS_INPUT
    "Missing name for redirect.", // MSE_MISSING_NAME_REDIRECT
    "Invalid null command.", // MSE_NULL_COMMAND
    "Command not found.", // MSE_COMMAND_NOT_FOUND
};

int ms_fail_parse(ms_shell_context_t *context, ms_error_t error,
    ms_token_t *token)
{
    if (context->is_interactive)
        my_dprintf(2, "%s\n", interactive_errs[error]);
    else {
        if (token == NULL)
            my_dprintf(2, "ParseError: %s\n", script_errs[error]);
        else {
            my_dprintf(2, "ParseError on line %d: %s\n", token->line,
                script_errs[error]);
            my_dprintf(2, "    at token %s\n", ms_token_to_str(token));
        }
    }
    return MYSH_ERROR;
}

int ms_fail(ms_shell_context_t *context, ms_error_t error)
{
    if (context->is_interactive)
        my_dprintf(2, "%s\n", interactive_errs[error]);
    else
        my_dprintf(2, "%s\n", script_errs[error]);
    return MYSH_ERROR;
}

int ms_error(char *error)
{
    my_dprintf(2, "%s\n", error);
    return MYSH_ERROR;
}
