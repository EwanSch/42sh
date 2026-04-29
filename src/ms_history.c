/*
** EPITECH PROJECT, 2025
** tcsh
** File description:
** ms_history.c
*/

#include "minishell1.h"
#include "minishell2.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

void fill_the_history(ms_shell_context_t *ctx, char *line)
{
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char time[MAX_TIME];

    if (!line || line[0] == '\0')
        return;
    if (t->tm_min < 10)
        my_snprintf(time, sizeof(time), "%d:0%d", t->tm_hour, t->tm_min);
    else
        my_snprintf(time, sizeof(time), "%d:%d", t->tm_hour, t->tm_min);
    ctx->history[ctx->history_index] = my_strdup(line);
    ctx->time[ctx->history_index] = my_strdup(time);
    ctx->history_index++;
}

int check_display(char *line, ms_shell_context_t *ctx)
{
    if (my_strcmp(line, "history") == 0) {
        fill_the_history(ctx, line);
        for (size_t i = 0; i < ctx->history_index; ++i) {
            my_printf("%6d %5s\t%s\n", i + 1, ctx->time[i], ctx->history[i]);
        }
        return 1;
    }
    return 0;
}

static int is_number_command(char *line)
{
    char *str = my_strstr(line, "!");

    if (!str)
        return 0;
    return my_isnum(*str + 1) ? 1 : 0;
}

char *expand_history(char *line, ms_shell_context_t *ctx)
{
    char *last_cmd = NULL;
    char *new_cmd = NULL;

    if (!line)
        return NULL;
    if (is_number_command(line)) {
        if (!number_case(line, ctx))
            return NULL;
        new_cmd = number_case(line, ctx);
    }
    if (my_strstr(line, HISTORY_CMD) != NULL) {
        if (!double_bang(line, ctx))
            return NULL;
        new_cmd = double_bang(line, ctx);
    }
    if (new_cmd) {
        free(line);
        line = new_cmd;
    }
    return line;
}
