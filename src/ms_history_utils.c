/*
** EPITECH PROJECT, 2025
** tcsh
** File description:
** ms_history_utils.c
*/

#include "minishell1.h"
#include "minishell2.h"
#include <stddef.h>
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
    int value = 0;

    value = index >= 0 ? value : 1;
    value = (ctx->history_index == 0 && value == 0) ? 2 : value;
    value = (!ctx->history[index] && value == 0) ? 3 : value;
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

int is_there_delimiter(char *line, char delim)
{
    for (size_t i = 0; line[i]; ++i)
        if (line[i] == delim)
            return 1;
    return 0;
}

char *del_delimiter(char *str, char delim)
{
    for (size_t i = 1; str[i]; ++i) {
        if (str[i] == delim)
            str[i] = '\0';
        return str;
    }
    return str;
}
