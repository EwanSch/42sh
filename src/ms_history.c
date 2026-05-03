/*
** EPITECH PROJECT, 2025
** tcsh
** File description:
** ms_history.c
*/

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
    safe_free(&ctx->history[MYSH_HISTORY_SIZE - 1]);
    ctx->history[0] = NULL;
    ctx->history[ctx->history_index + 1] = my_strdup(line);
    safe_free(&ctx->time[MYSH_HISTORY_SIZE - 1]);
    ctx->time[ctx->history_index + 1] = my_strdup(time);
    ctx->history_index++;
}

int check_display(char *line, ms_shell_context_t *ctx)
{
    if (my_strcmp(line, "history") == 0) {
        fill_the_history(ctx, line);
        for (size_t i = 1; i < ctx->history_index + 1; ++i) {
            my_printf("%6d %5s\t%s\n", i, ctx->time[i], ctx->history[i]);
        }
        return 1;
    }
    fill_the_history(ctx, line);
    return 0;
}

static int is_num_command(char *line)
{
    char *str = my_strstr(line, "!");
    int value = 0;

    if (!str)
        return 0;
    if (str[1] == '-') {
        value = my_isnum(str[2]);
    } else {
        value = my_isnum(str[1]);
    }
    return value;
}

static int set_flag_in_line(char *line, ms_shell_context_t *ctx)
{
    int count = 0;
    char **arr = my_explode(line, " \t");

    if (!arr)
        return count;
    for (size_t i = 0; arr[i]; ++i) {
        if (is_num_command(arr[i]) || my_strstr(arr[i], HISTORY_CMD) != NULL)
            count++;
    }
    for (size_t i = 0; arr[i]; ++i)
        safe_free(&arr[i]);
    safe_free(&arr);
    return count;
}

char *expand_history(char *line, ms_shell_context_t *ctx)
{
    char *last_cmd = NULL;
    char *new_cmd = NULL;
    int is_command = 0;

    if (!line)
        return NULL;
    is_command = set_flag_in_line(line, ctx);
    if (is_num_command(line)) {
        new_cmd = number_case(line, ctx);
        if (!new_cmd)
            return NULL;
    }
    if (my_strstr(line, HISTORY_CMD) != NULL) {
        new_cmd = double_bang(line, ctx);
        if (!new_cmd)
            return NULL;
    }
    if (is_command > 0)
        new_cmd = expand_history(new_cmd, ctx);
    return new_cmd != NULL ? free_secure_switch(new_cmd) : line;
}
