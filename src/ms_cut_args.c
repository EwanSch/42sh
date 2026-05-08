/*
** EPITECH PROJECT, 2026
** minishell1
** File description:
** MiniShell translation unit
** containing commands related
** to current dir manipulation
** Author:
** Amélie Ambleton--Guth
** February 2026
*/

#include <string.h>

#include "ms_builtins.h"
#include "minishell2.h"
#include "benjalib/my.h"

static int push_nbr(ms_expre_word_t *data,
    ms_expression_t *expre, char *args, int i)
{
    data->parenth = expre->paren_depth;
    data->tokenid = expre->tokenid;
    ll_push(&expre->expre_list, data);
    if (args[i])
        each_word(&args[i], expre, expre->tokenid);
    return 1;
}

void o_get_val(ms_expre_word_t *data, char *args, int i)
{
    char *err = NULL;
    int val = 0;

    data->tokenisc = strndup(args, i);
    val = strtol(args, &err, 10);
    if (!*err)
        data->tokenisi = val;
}

void c_get_val(ms_expre_word_t *data, char *args, int i)
{
    char *err = NULL;
    int val = 0;

    data->tokenisc = strndup(args, i);
    val = strtol(data->tokenisc, &err, 10);
    if (!*err)
        data->tokenisi = val;
}

int open_para(char *args, int i, ms_expre_word_t *data, ms_expression_t *expre)
{
    if (args[i] == '(') {
        if (i != 0) {
            o_get_val(data, args, i);
            return push_nbr(data, expre, args, i);
        }
        free(data);
        if (i + 1 == expre->word_size) {
            expre->paren_depth++;
            return 1;
        }
        expre->paren_depth++;
        if (args[i + 1])
            each_word(&args[i + 1], expre, expre->tokenid);
        return 1;
    }
    return 0;
}

int close_para(char *args, int i, ms_expre_word_t *data,
    ms_expression_t *expre)
{
    if (args[i] == ')') {
        if (i != 0) {
            c_get_val(data, args, i);
            return push_nbr(data, expre, args, i);
        }
        free(data);
        if (i + 1 == expre->word_size) {
            expre->paren_depth--;
            return 1;
        }
        expre->paren_depth--;
        if (args[i + 1])
            each_word(&args[i + 1], expre, expre->tokenid);
        return 1;
    }
    return 0;
}

int each_word(char *args, ms_expression_t *expre, int curr)
{
    ms_expre_word_t *data = calloc(1, sizeof(struct ms_expre_word_s));
    char *err = NULL;
    int val = 0;

    expre->tokenid = curr;
    data->tokenisi = -1;
    expre->word_size = strlen(args);
    for (int i = 0; args[i]; i++)
        if (open_para(args, i, data, expre) || close_para(args, i, data, expre))
            return 0;
    val = strtol(args, &err, 10);
    if (expre->max_depth < expre->paren_depth)
        expre->max_depth = expre->paren_depth;
    if (!*err)
        data->tokenisi = val;
    data->tokenisc = strdup(args);
    data->parenth = expre->paren_depth;
    data->tokenid = expre->tokenid;
    ll_push(&expre->expre_list, data);
    return 0;
}
