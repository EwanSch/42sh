/*
** EPITECH PROJECT, 2026
** minishell1
** File description:
** MiniShell utils
** Author:
** Amélie Ambleton--Guth
*/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "minishell1.h"
#include "minishell2.h"
#include "var_substitution.h"
#include "benjalib.h"

char *del_start_str(char *str, int amount)
{
    int str_len = 0;
    int futur_size = 0;

    if (!str || str[0] == 0)
        return NULL;
    str_len = my_strlen(str);
    if (amount > str_len || amount < 0)
        return NULL;
    futur_size = str_len - amount;
    for (int i = 0; i < futur_size; i++)
        str[i] = str[i + amount];
    str[futur_size] = 0;
    return str;
}

char *list_to_str(list_t *list)
{
    char *str = NULL;
    int size = 0;
    list_t *tmp;

    while (list) {
        size += strlen((char *)list->data);
        str = my_recalloc(str, sizeof(char) * (size + 1));
        if (!str)
            return NULL;
        strcat(str, list->data);
        free(list->data);
        tmp = list;
        list = list->next;
        free(tmp);
    }
    return str;
}

static int resize_var(int i, char *key, var_utils_t *utils)
{
    if (key[1] == '{' && utils->str[i + 1] == '}') {
        del_start_str(key, 2);
        key[strlen(key)] = 0;
        utils->word_size++;
    } else
        del_start_str(key, 1);
    return 0;
}

int var_from_km(char *var, char **key, ms_shell_context_t *ctx)
{
    if (!var)
        var = km_get_or_default(*key, ctx->env, NULL);
    if (!var) {
        printf("%s: Undefined variable.\n", *key);
        free(*key);
        return 84;
    }
    free(*key);
    return 0;
}

static int get_val_data(int i, var_utils_t *utils, list_t *list,
    ms_shell_context_t *ctx)
{
    char *var;
    char *key = NULL;

    if (utils->dollar_pos != -1) {
        key = strndup(&utils->str[utils->dollar_pos],
            i - utils->dollar_pos + 1);
        utils->word_size += i - utils->dollar_pos + 1;
        if (resize_var(i, key, utils))
            return 84;
        utils->dollar_pos = -1;
        var = km_get_or_default(key, ctx->variables, NULL);
        if (var_from_km(var, &key, ctx))
            return 84;
        var = my_strdup(var);
        ll_push(&list, var);
    }
    return 0;
}

static int end_of_var(int i, var_utils_t *utils,
    list_t *list, ms_shell_context_t *ctx)
{
    if (!utils->str[i + 1] ||
        !(isalnum(utils->str[i + 1]) || utils->str[i + 1] == '_')) {
        if (get_val_data(i, utils, list, ctx))
            return 84;
    }
    return 0;
}

void each_quote(char str, int *which_quote)
{
    if (str == '\"' && *which_quote != 1) {
        if (*which_quote == 2)
            *which_quote = 0;
        else
            *which_quote = 2;
    }
    if (str == '\'' && *which_quote != 2) {
        if (*which_quote == 1)
            *which_quote = 0;
        else
            *which_quote = 1;
    }
}

static int get_value(int i, var_utils_t *utils, int which_quote, list_t **list)
{
    if (utils->str[i] == '$' && which_quote != 1) {
        if (utils->str[i + 1] == '\n' || utils->str[i + 1] == ' ') {
            return 1;
        }
        ll_push(list, strndup(&utils->str[utils->word_size],
                i - utils->word_size));
        utils->dollar_pos = i;
        utils->word_size = i;
    }
    return 0;
}

char *var_sub(char *str, ms_shell_context_t *ctx)
{
    list_t *list = NULL;
    var_utils_t utils = {0};
    int which_quote = 0;

    utils.dollar_pos = -1;
    utils.str = str;
    for (int i = 0; str[i]; i++) {
        each_quote(str[i], &which_quote);
        if (end_of_var(i, &utils, list, ctx))
            return NULL;
        if (get_value(i, &utils, which_quote, &list))
            continue;
        if (str[i + 1] == '\0')
            ll_push(&list, strndup(&str[utils.word_size],
                    i - utils.word_size + 1));
    }
    return list_to_str(list);
}
