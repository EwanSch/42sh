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

static int is_equal(char *str)
{
    for (int i = 0; str[i] != '\0'; ++i)
        if (str[i] == '=')
            return i;
    return -1;
}

static void value_nb_neg(int *nb, int i, char **value, char **args)
{
    *nb = is_equal(args[i]);
    if (*nb == -1)
        *value = strdup(args[i]);
}

static void div_else_if(char **args, int *i, char **value)
{
    if (args[*i + 1]) {
        *value = strdup(args[*i + 1]);
        *i += 1;
    }
}

void check_equal(char **args, int *i, char **name, char **value)
{
    int nb = 0;

    value_nb_neg(&nb, *i, value, args);
    if (nb != -1) {
        *name = malloc(sizeof(char) * nb + 1);
        if (!(*name))
            return;
        *name = strncpy(*name, args[*i], nb);
        (*name)[nb] = '\0';
        nb += 1;
        if (args[*i][nb] != '\0')
            *value = strdup(&args[*i][nb]);
        return;
    } else if (nb != -1 && args[*i + 1] && !strcmp(args[*i + 1], "=")) {
        *name = strdup(args[*i]);
        *i += 1;
        div_else_if(args, i, value);
    }
}

static int div_check_error_set(char **args, int i)
{
    if (args[i + 1] && args[i + 1][strlen(args[i + 1]) - 1] == '=') {
        dprintf(2, "set: Variable name must begin with a letter.\n");
        return 1;
    }
    if (args[i + 1] && args[i][strlen(args[i]) - 1] == '=') {
        dprintf(2, "set: Variable name must begin with a letter.\n");
        return 1;
    }
    return 0;
}

static int check_error_set(char **args, int i)
{
    if (args[i][strlen(args[i])] == '=') {
        dprintf(2, "set: Variable name must begin with a letter.\n");
        return 1;
    }
    if (is_equal(args[i]) == -1 && args[i + 1] &&
        is_equal(args[i + 1]) == -1) {
        dprintf(2, "set: Variable name must begin with a letter.\n");
        return 1;
    }
    if (args[i + 1] && args[i + 1][0] == '=' && args[i + 1][1] != '\0') {
        dprintf(2, "set: Variable name must begin with a letter.\n");
        return 1;
    }
    if (div_check_error_set(args, i))
        return 1;
    return 0;
}

static int check_solo(char **args, int i, char **name)
{
    int j = 0;

    for (; args[j]; ++j);
    if (j > 2)
        return 0;
    if (j < 1)
        return 0;
    if (args[i][strlen(args[i]) - 1] == '=') {
        *name = malloc(sizeof(char) * strlen(args[i]));
        if (!(*name))
            return 0;
        *name = strncpy(*name, args[i], strlen(args[i]) - 1);
        (*name)[strlen(args[i]) - 1] = '\0';
        return 1;
    }
    return 0;
}

int ms_bi_set(ms_shell_context_t *context, char **args)
{
    char *value = NULL;
    char *name;
    int idx = 0;

    for (int i = 0; args[i]; ++i) {
        if (args[i][0] == '-')
            continue;
        if (check_error_set(args, i))
            return 1;
        if (check_solo(args, i, &name))
            break;
        check_equal(args, &i, &name, &value);
    }
    if (value == NULL)
        value = strdup("");
    km_set(name, value, &context->variables);
    safe_free(&name);
    safe_free(&value);
    return 0;
}
