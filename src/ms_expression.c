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

void free_func(list_t **buf)
{
    free(((ms_expre_word_t *)(*buf)->next->next->data)->tokenisc);
    free((*buf)->next->next);
    free(((ms_expre_word_t *)(*buf)->next->data)->tokenisc);
    free((*buf)->next);
    free(((ms_expre_word_t *)(*buf)->data)->tokenisc);
    free(*buf);
}

static int change_depth(list_t **buf, int *was_here, ms_expression_t *expre)
{
    ms_expre_word_t *data;
    ms_expre_word_t *data_next;

    data = (*buf)->data;
    if ((*buf)->next) {
        data_next = (ms_expre_word_t *)(*buf)->next->data;
    } else
        return 1;
    if (data->parenth != data_next->parenth &&
        (!(*buf)->next->next ||
            data_next->parenth != ((ms_expre_word_t *)
                (*buf)->next->next->data)->parenth) &&
        data_next->parenth == expre->max_depth) {
        data_next->parenth--;
        *was_here = 1;
    }
    return 0;
}

int check_depth(ms_expression_t *expre)
{
    list_t **buf = &expre->expre_list;
    int was_here = 0;

    if ((*buf)->next && ((ms_expre_word_t *)(*buf)->data)->parenth != ((ms_expre_word_t *)(*buf)->next->data)->parenth) {
        ((ms_expre_word_t *)(*buf)->data)->parenth--;
        was_here = 1;
    }

    for (; *buf; buf = &(*buf)->next) {
        if (change_depth(buf, &was_here, expre))
            break;
    }
    if (was_here) {
        expre->max_depth--;
        if (do_expre(expre))
            return 84;
    }
    return 0;
}

void free_all_ll(void *data)
{
    ms_expre_word_t *expre = data;

    safe_free(&expre->tokenisc);
    free(expre);
}

static long get_value(char **args, bool *err, int *cmd_place)
{
    ms_expression_t expre = {0};
    int size = 0;
    int res = 0;

    for (; args[size]; size++)
        each_word(args[size], &expre, size);
    *cmd_place = size;
    if (((ms_expre_word_t *)expre.expre_list->data)->tokenisi == -1) {
        *err = true;
        ll_free_linked(expre.expre_list, free_all_ll);
        return 84;
    }
    if (do_expre(&expre) || expre.paren_depth) {
        *err = true;
        ll_free_linked(expre.expre_list, free_all_ll);
        return 84;
    }
    if (expre.cmd_place)
        *cmd_place = expre.cmd_place;
    res = ((ms_expre_word_t *)expre.expre_list->data)->tokenisi;
    ll_free_linked(expre.expre_list, free_all_ll);
    return res;
}

static int fill_arr(char ***arr, char *str, int size)
{
    *arr = realloc(*arr, sizeof(char *) * (size + 1));
    if (*arr == NULL)
        return 84;
    (*arr)[size - 1] = strdup(str);
    (*arr)[size] = NULL;
    return 0;
}

static int fuse_data(char **left, char **right, int comp, int *cmd_place)
{
    int left_size = 0;
    bool err = false;
    long val1 = get_value(left, &err, cmd_place);
    int place = *cmd_place;
    long val2 = get_value(right, &err, cmd_place);

    for (; left[left_size]; left_size++);
    if (place != left_size) {
        *cmd_place = place - 1;
        return val1;
    }
    *cmd_place += place;
    if (err)
        return -1;
    if (comp == 1 && val1 == val2)
        return 1;
    if (comp == 1 && val1 != val2)
        return 0;
    if (comp == 2 && val1 != val2)
        return 1;
    if (comp == 2 && val1 == val2)
        return 0;
    return -1;
}

int reset_arr(int *comp, char **args, int i)
{
    if (!strcmp(args[i], "==")) {
        *comp = 1;
        return 1;
    }
    if (!strcmp(args[i], "!=")) {
        *comp = 2;
        return 1;
    }
    return 0;
}

char *fill_cmd(char **args, int cmd_place)
{
    if (!cmd_place)
        return NULL;
    return array_to_str(&args[cmd_place]);
}

static int fill_both(char ***left, char ***right, int *comp, char **args)
{
    int size = 0;
    int found = 0;

    for (int i = 0; args[i]; i++) {
        if (reset_arr(comp, args, i)) {
            found = 1;
            size = 0;
            i++;
        }
        size++;
        if (!found)
            fill_arr(left, args[i], size);
        else
            fill_arr(right, args[i], size);
    }
}

long ms_expression(char **args, char **cmd, bool *err)
{
    int comp = 0;
    char **right = NULL;
    char **left = NULL;
    long res = 0;
    int cmd_place = 0;

    fill_both(&left, &right, &comp, args);
    if (!comp) {
        res = get_value(left, err, &cmd_place);
        *cmd = fill_cmd(args, cmd_place);
        free_str_arr(left);
        if (err) {
            free (*cmd);
            *cmd = NULL;
        }
        return res;
    }
    res = fuse_data(left, right, comp, &cmd_place);
    *cmd = fill_cmd(args, cmd_place + 1);
    free_str_arr(left);
    free_str_arr(right);
    if (res == -1) {
        free(*cmd);
        *cmd = NULL;
        *err = true;
    }
    else
        return res;
    return 0;
}
