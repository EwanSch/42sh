/*
** EPITECH PROJECT, 2025
** tcsh
** File description:
** ms_history_case.c
*/

#include "minishell1.h"
#include "minishell2.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>

static char *replace_str(char *str, char *to_replace, char *replacement)
{
    char buffer[MAX_CMD] = {0};
    char *part = my_strstr(str, to_replace);
    char *new_line = NULL;

    if (!part)
        return my_strdup(str);
    my_strncpy(buffer, str, part - str);
    buffer[part - str] = '\0';
    my_strcat(buffer, replacement);
    my_strcat(buffer, part + my_strlen(to_replace));
    safe_free(&str);
    new_line = my_strdup(buffer);
    return new_line;
}

static int get_the_num(char *line, ms_shell_context_t *ctx, int *is_less)
{
    size_t i = 0;
    char *str = my_strdup(line);
    char *tmp = my_strstr(str, "!");
    int value = 0;

    tmp += 1;
    if (tmp[0] == '-') {
        value = ctx->history_index;
        tmp += 1;
        for (; tmp[i] != ' ' && tmp[i] != '\t' && tmp[i] != '\0'; ++i);
        tmp[i] = '\0';
        value -= my_getnbr(tmp);
        *is_less -= my_getnbr(tmp);
    } else {
        for (; tmp[i] != ' ' && tmp[i] != '\t' && tmp[i] != '\0'; ++i);
        tmp[i] = '\0';
        value = my_getnbr(tmp);
    }
    safe_free(&str);
    return value;
}

static char *get_the_string(char *line, int *is_string)
{
    char *tmp = my_strdup(line);
    size_t i = 1;

    tmp = my_strstr(line, "!");
    if (!tmp)
        return line;
    if (tmp[1] == '?' && is_there_delimiter(tmp + 2, '?')) {
        i++;
        for (; tmp[i] != '?'; ++i);
        tmp[i + 1] = '\0';
        *is_string = 1;
        return tmp;
    }
    if (tmp[1] == '?') {
        i++;
        for (; tmp[i] != ' ' && tmp[i] != '\t' && tmp[i] != '\0'; ++i);
        tmp[i] = '\0';
        *is_string = 1;
        return tmp;
    }
    *is_string = 0;
    for (; tmp[i] != ' ' && tmp[i] != '\t' && tmp[i] != '\0'; ++i);
    tmp[i] = '\0';
    return tmp;
}

static char *find_by_str(char *to_find, ms_shell_context_t *ctx, int is_str)
{
    if (!to_find)
        return NULL;
    to_find += 1;
    if (is_str)
        to_find += 1;
    for (size_t i = 1; is_str && to_find[i]; ++i) {
        if (to_find[i] == '?')
            to_find[i] = '\0';
    }
    for (size_t i = 1; i < ctx->history_index + 1; ++i) {
        if (is_str && my_strstr(ctx->history[i], to_find) != NULL) {
            my_printf("%s\n", ctx->history[i]);
            return ctx->history[i];
        }
        if (!is_str && my_strncmp(ctx->history[i], to_find, my_strlen(to_find)) == 0) {
            my_printf("%s\n", ctx->history[i]);
            return ctx->history[i];
        }
    }
    return NULL;
}

char *str_case(char *line, ms_shell_context_t *ctx)
{
    char *new_cmd = NULL;
    char *last_cmd = NULL;
    int is_string = 0;
    char *to_replace = get_the_string(line, &is_string);

    last_cmd = find_by_str(to_replace, ctx, is_string);
    if (!last_cmd) {
        my_dprintf(2, MS_NO_HISTORY2, del_delimiter(to_replace, '?'));
        return NULL;
    }
    line = replace_str(line, to_replace, last_cmd);
    return line;
}

char *number_case(char *line, ms_shell_context_t *ctx)
{
    char *new_cmd = NULL;
    char *last_cmd = NULL;
    char to_replace[CMD_STRING];
    int is_less = 0;
    int value = get_the_num(line, ctx, &is_less);

    if (no_history(ctx, (is_less < 0) ? is_less : value))
        return NULL;
    last_cmd = ctx->history[value];
    my_snprintf(to_replace, sizeof(to_replace),
        "!%d", (is_less < 0) ? is_less : value);
    line = replace_str(line, to_replace, last_cmd);
    return line;
}

char *double_bang(char *line, ms_shell_context_t *ctx)
{
    char *new_cmd = NULL;
    char *last_cmd = NULL;

    if (no_history(ctx, ctx->history_index))
        return NULL;
    last_cmd = ctx->history[ctx->history_index];
    line = replace_str(line, HISTORY_CMD, last_cmd);
    return line;
}
