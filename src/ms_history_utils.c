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

char *free_secure_switch(void *new_command)
{
    char *tmp = NULL;
    void **full_cmd = new_command;

    if (*full_cmd != NULL) {
        tmp = my_strdup(*full_cmd);
        free(*full_cmd);
        *full_cmd = NULL;
        return tmp;
    }
    return NULL;
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
    for (size_t i = 2; str[i]; ++i) {
        if (str[i] == delim)
            str[i] = '\0';
    }
    return str + 2;
}
