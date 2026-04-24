/*
** EPITECH PROJECT, 2025
** tcsh
** File description:
** ms_history.c
*/

#include "minishell1.h"
#include "minishell2.h"
#include <stdio.h>
#include <string.h>

void fill_the_history(ms_shell_context_t *ctx, ms_line_editor_t *lined)
{
    ctx->history[ctx->history_index] =
        my_strcmp(lined->history[1], "!!") == 0 ?
        my_strdup(lined->history[2]) : my_strdup(lined->history[1]);
    ctx->history_index++;
}

int error_case(ms_shell_context_t *ctx)
{
    if (ctx->history[1] == NULL) {
        my_dprintf(2, "%s\n", MS_NO_HISTORY);
        return 1;
    }
    return 0;
}

char *expand_history(char *line, ms_shell_context_t *ctx)
{
    if (!line)
        return NULL;
    if (my_strcmp(line, "!!") == 0) {
        if (error_case(ctx))
            return NULL;
        free(line);
        line = NULL;
        line = my_strdup(ctx->history[ctx->history_index - 1]);
    }
    return line;
}