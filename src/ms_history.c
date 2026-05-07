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

void ms_free_history(ms_shell_context_t *ctx)
{
    for (int i = 0; i < ctx->history_index; ++i) {
        safe_free(&ctx->history[i + 1]);
        safe_free(&ctx->time[i + 1]);
    }
}

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
    return 0;
}

static int target_is_num(char *line)
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

static int target_is_str(char *line)
{
    char *str = my_strstr(line, "!");

    if (!str || !str[1])
        return 0;
    if (str[1] == '?' && str[2])
        return my_isalpha(str[2]);
    return my_isalpha(str[1]);
}

static int set_flag_in_line(char *line, ms_shell_context_t *ctx)
{
    int count = 0;
    char **arr = my_explode(line, " \t");

    if (!arr)
        return count;
    for (size_t i = 0; arr[i]; ++i) {
        if (target_is_num(arr[i]) || my_strstr(arr[i], HISTORY_CMD) != NULL)
            count++;
    }
    for (size_t i = 0; arr[i]; ++i)
        safe_free(&arr[i]);
    safe_free(&arr);
    return count;
}

static char *div_expand_history(char *line, ms_shell_context_t *ctx,
    int *checked)
{
    char *new_line = NULL;

    if (my_strstr(line, HISTORY_CMD) != NULL) {
        *checked = 1;
        new_line = double_bang(line, ctx);
    }
    if (target_is_num(line)) {
        *checked = 1;
        new_line = number_case(line, ctx);
    }
    if (target_is_str(line)) {
        *checked = 1;
        new_line = str_case(line, ctx);
    }
    if (*checked && !new_line)
        return NULL;
    return new_line;
}

char *expand_history(char *line, ms_shell_context_t *ctx)
{
    char *new_line = NULL;
    int is_command = 0;
    int checked = 0;

    if (!line)
        return NULL;
    is_command = set_flag_in_line(line, ctx);
    new_line = div_expand_history(line, ctx, &checked);
    if (!new_line && checked)
        return NULL;
    if (is_command > 0 && new_line != NULL)
        new_line = expand_history(new_line, ctx);
    if (new_line != NULL) {
        line = my_strdup(new_line);
        my_printf("%s\n", line);
        safe_free(&new_line);
    }
    if (check_display(line, ctx))
        return NULL;
    return line;
}
