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

    if ((*buf)->next && ((ms_expre_word_t *)(*buf)->data)->parenth !=
        ((ms_expre_word_t *)(*buf)->next->data)->parenth) {
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

void free_func(list_t **buf)
{
    free(((ms_expre_word_t *)(*buf)->next->next->data)->tokenisc);
    free((*buf)->next->next->data);
    free((*buf)->next->next);
    free(((ms_expre_word_t *)(*buf)->next->data)->tokenisc);
    free((*buf)->next->data);
    free((*buf)->next);
    free(((ms_expre_word_t *)(*buf)->data)->tokenisc);
    free((*buf)->data);
    free(*buf);
}

static long calc(long val1, long val2, int ope)
{
    switch (ope) {
        case 1:
            return (val1 + val2);
        case 2:
            return (val1 - val2);
        case 3:
            return (val1 * val2);
        case 4:
            return (val1 / val2);
        case 5:
            return (val1 % val2);
        default:
            return 0;
    }
}

static void push_data(list_t ***buf, ms_expression_t *expre, long val1)
{
    ms_expre_word_t *res = {0};
    list_t *tmp = NULL;

    res = calloc(1, sizeof(ms_expre_word_t));
    res->parenth = expre->max_depth;
    res->tokenisi = val1;
    tmp = (**buf)->next->next->next;
    free_func(*buf);
    **buf = NULL;
    ll_push(*buf, res);
    (**buf)->next = tmp;
}

int check_valid_nbr(list_t ***buf)
{
    char *err;

    if (((ms_expre_word_t *)(**buf)->next->next->data)->tokenisc) {
        strtol(((ms_expre_word_t *)(**buf)->next->next->data)->tokenisc,
            &err, 10);
        if (*err)
            return 84;
    }
    return 0;
}

int impossible_calc(int ope, long val2)
{
    if (ope == 4 && !val2) {
        dprintf(1, "Division by 0.\n");
        return 42;
    }
    if (ope == 5 && !val2) {
        dprintf(1, "Mod by 0.\n");
        return 42;
    }
    return 0;
}

int calculate(list_t ***buf, ms_expression_t *expre, int ope, int *was_here)
{
    long val1 = 0;
    long val2 = 0;

    if (ope) {
        val1 = ((ms_expre_word_t *)(**buf)->data)->tokenisi;
        if (!(**buf)->next->next)
            return 84;
        if (check_valid_nbr(buf))
            return 84;
        val2 = ((ms_expre_word_t *)(**buf)->next->next->data)->tokenisi;
        if (impossible_calc(ope, val2))
            return 42;
        val1 = calc(val1, val2, ope);
        push_data(buf, expre, val1);
        *was_here = 1;
    }
    return 0;
}
