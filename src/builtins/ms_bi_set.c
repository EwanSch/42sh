/*
** EPITECH PROJECT, 2025
** 42sh
** File description:
** ms_bi_set.c
*/

#include "minishell1.h"
#include "minishell2.h"
#include "shell.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static int args_len(char **args)
{
    int i = 0;

    while (args[i])
        ++i;
    return i;
}

static int index_of_equal(const char *str)
{
    for (int i = 0; str[i] != '\0'; ++i)
        if (str[i] == '=')
            return i;
    return -1;
}

int check_sec_error(const char *cur, const char *next)
{
    if (next && next[0] == '=' && next[1] != '\0') {
        dprintf(2, "set: Variable name must begin with a letter.\n");
        return 1;
    }
    if (next && cur[strlen(cur) - 1] == '='
        && next[0] == '=') {
        dprintf(2, "set: Variable name must begin with a letter.\n");
        return 1;
    }
    return 0;
}

static int check_error_set(char **args, int i)
{
    const char *cur = args[i];
    const char *next = args[i + 1];
    int eq = index_of_equal(cur);

    if (eq == 0) {
        dprintf(2, "set: Variable name must begin with a letter.\n");
        return 1;
    }
    if (eq == -1 && next && index_of_equal(next) == -1
        && next[0] != '=') {
        dprintf(2, "set: Variable name must begin with a letter.\n");
        return 1;
    }
    if (check_sec_error(cur, next))
        return 1;
    return 0;
}

static int handle_solo_trailing_eq(char **args, int i, char **name, int total)
{
    int len = strlen(args[i]);

    if (total > 2 || len < 2 || args[i][len - 1] != '=')
        return 0;
    *name = strndup(args[i], len - 1);
    return 1;
}

static void parse_assignment(char **args, int *i, char **name, char **value)
{
    int eq = index_of_equal(args[*i]);

    if (eq > 0) {
        *name = strndup(args[*i], eq);
        if (args[*i][eq + 1] != '\0')
            *value = strdup(&args[*i][eq + 1]);
        return;
    }
    *name = strdup(args[*i]);
    if (!args[*i + 1] || strcmp(args[*i + 1], "=") != 0)
        return;
    *i += 1;
    if (args[*i + 1]) {
        *value = strdup(args[*i + 1]);
        *i += 1;
    }
}

void free_set(char **name, char **value)
{
    safe_free(name);
    safe_free(value);
}

int ms_bi_set(ms_shell_context_t *context, char **args)
{
    char *value = NULL;
    char *name = NULL;
    int total = args_len(args);

    for (int i = 0; args[i]; ++i) {
        if (args[i][0] == '-')
            continue;
        if (check_error_set(args, i))
            return 1;
        if (handle_solo_trailing_eq(args, i, &name, total))
            break;
        parse_assignment(args, &i, &name, &value);
    }
    if (!name)
        return 1;
    if (!value)
        value = strdup("");
    km_set(name, value, &context->variables);
    free_set(&name, &value);
    return 0;
}
