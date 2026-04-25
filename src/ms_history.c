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

// =================================================================================== file separator
// =================================================================================== file separator
void fill_the_history(ms_shell_context_t *ctx, char *line)
{
    if (!line || line[0] == '\0')
        return;
    if (ctx->history_index > 0 && 
        my_strcmp(ctx->history[ctx->history_index - 1], line) == 0) {
        return;
    }
    ctx->history[ctx->history_index] = my_strdup(line);
    ctx->history_index++;
}

static int check_display(char *line, ms_shell_context_t *ctx)
{
    if (my_strcmp(line, "history") == 0) {
        fill_the_history(ctx, line);
        for (size_t i = 0; i < ctx->history_index; ++i) {
            my_printf("%6d %5s\t%s\n", i + 1, "0:00", ctx->history[i]);
        }
        return 1;
    }
    return 0;
}

char *expand_history(char *line, ms_shell_context_t *ctx)
{
    if (!line)
        return NULL;
    if (my_strcmp(line, "!!") == 0) {
        if (ctx->history_index == 0) {
            my_dprintf(2, MS_NO_HISTORY, ctx->history_index);
            return NULL;
        }
        free(line);
        line = NULL;
        line = my_strdup(ctx->history[ctx->history_index - 1]);
    }
    if (check_display(line, ctx))
        return NULL;
    return line;
}
