/*
** EPITECH PROJECT, 2025
** tcsh
** File description:
** ms_history_case.c
*/

#include "minishell1.h"
#include "minishell2.h"
#include <stdio.h>
#include <string.h>

char *free_secure_switch(char *new_command)
{
    char *tmp = NULL;

    if (new_command != NULL) {
        tmp = my_strdup(new_command);
        safe_free(&new_command);
        return tmp;
    }
}

int no_history(ms_shell_context_t *ctx, int index)
{
    int value = -1;

    value = index < 0 ? 1 : value;
    value = ctx->history_index == 0 ? 2 : value;
    value = !ctx->history[index] ? 3 : value;
    if (value == 1) {
        my_dprintf(2, MS_NO_HISTORY, index + 1);
        return 1;
    }
    if (value == 3) {
        my_dprintf(2, MS_NO_HISTORY, index);
        return 1;
    }
    if (value == 2) {
        my_dprintf(2, MS_NO_HISTORY, ctx->history_index);
        return 1;
    }
    return 0;
}

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
    new_line = my_strdup(buffer);
    return new_line;
}

static int get_num_in_line(char *line, ms_shell_context_t *ctx, int *is_less)
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

char *number_case(char *line, ms_shell_context_t *ctx)
{
    char *new_cmd = NULL;
    char *last_cmd = NULL;
    char to_edit[CMD_STRING];
    int is_less = 0;
    int value = get_num_in_line(line, ctx, &is_less);

    if (no_history(ctx, (is_less < 0) ? is_less : value))
        return NULL;
    last_cmd = ctx->history[value];
    my_snprintf(to_edit, sizeof(to_edit),
        "!%d", (is_less < 0) ? is_less : value);
    new_cmd = replace_str(line, to_edit, last_cmd);
    return new_cmd != NULL ? free_secure_switch(new_cmd) : line;
}

char *double_bang(char *line, ms_shell_context_t *ctx)
{
    char *new_cmd = NULL;
    char *last_cmd = NULL;

    if (no_history(ctx, 0))
        return NULL;
    last_cmd = ctx->history[ctx->history_index - 1];
    new_cmd = replace_str(line, HISTORY_CMD, last_cmd);
    return new_cmd != NULL ? free_secure_switch(new_cmd) : line;
}
