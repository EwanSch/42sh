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

static int no_history(char *line, ms_shell_context_t *ctx)
{
    if (ctx->history_index == 0) {
        my_dprintf(2, MS_NO_HISTORY, ctx->history_index);
        return 1;
    }
    return 0;
}

static char *single_case(char *line, ms_shell_context_t *ctx)
{
    if (no_history(line, ctx))
        return NULL;
    free(line);
    line = NULL;
    line = my_strdup(ctx->history[ctx->history_index - 1]);
    return line;
}

static char *replace_str(char *str, char *to_replace, char *replacement)
{
    char buffer[MAX_CMD] = {0};
    char *part = my_strstr(str, to_replace);

    if (!part)
        return my_strdup(str);
    my_strncpy(buffer, str, part - str);
    buffer[part - str] = '\0';
    my_strcat(buffer, replacement);
    my_strcat(buffer, part + my_strlen(to_replace));
    return my_strdup(buffer);
}

char *expand_history(char *line, ms_shell_context_t *ctx)
{
    char *last_cmd = NULL;
    char *new_line = NULL;

    if (!line)
        return NULL;
    if (my_strcmp(line, HISTORY_CMD) == 0)
        return single_case(line, ctx);
    if (my_strstr(line, HISTORY_CMD) != NULL) {
        if (no_history(line, ctx))
            return NULL;
        last_cmd = ctx->history[ctx->history_index - 1];
        new_line = replace_str(line, HISTORY_CMD, last_cmd);
    } else
        return line;
    free(line);
    line = new_line;
    return line;
}
