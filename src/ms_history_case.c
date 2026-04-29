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

int no_history(char *line, ms_shell_context_t *ctx)
{
    if (ctx->history_index == 0) {
        my_dprintf(2, MS_NO_HISTORY, ctx->history_index);
        return 1;
    }
    return 0;
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

char *number_case(char *line, ms_shell_context_t *ctx)
{
    char *new_cmd = NULL;
    char *last_cmd = NULL;
    char to_edit[CMD_STRING];
    int num = 0;

    if (no_history(line, ctx))
        return NULL;
    last_cmd = ctx->history[ctx->history_index - 1];
    my_snprintf(to_edit, sizeof(to_edit), "!%d", num);
    new_cmd = replace_str(line, to_edit, last_cmd);
    return new_cmd ? new_cmd : line;
}

char *double_bang(char *line, ms_shell_context_t *ctx)
{
    char *new_cmd = NULL;
    char *last_cmd = NULL;

    if (no_history(line, ctx))
        return NULL;
    last_cmd = ctx->history[ctx->history_index - 1];
    new_cmd = replace_str(line, HISTORY_CMD, last_cmd);
    return new_cmd ? new_cmd : line;
}