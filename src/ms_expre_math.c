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

char *find_str(void *data)
{
    ms_expre_word_t *expre = data;

    return expre->tokenisc;
}

static int search_op(list_t ***buf, int(get_opera)(char *),
    int *ope, ms_expression_t *expre)
{
    if (((ms_expre_word_t *)(**buf)->next->data)->tokenisc) {
        if (((ms_expre_word_t *)(**buf)->next->data)->parenth
            != expre->max_depth) {
            *buf = &(**buf)->next;
            return 1;
        }
        *ope = get_opera(((ms_expre_word_t *)(**buf)->next->data)->tokenisc);
        if (*ope == -1) {
            expre->cmd_place =
                ((ms_expre_word_t *)(**buf)->next->data)->tokenid;
            return 2;
        }
    }
    return 0;
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

static int check_valid_nbr(list_t ***buf)
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
        if (ope == 4 && !val2) {
            dprintf(1, "Division by 0.\n");
            return 42;
        }
        if (ope == 5 && !val2) {
            dprintf(1, "Mod by 0.\n");
            return 42;
        }
        val1 = calc(val1, val2, ope);
        push_data(buf, expre, val1);
        *was_here = 1;
    }
    return 0;
}

int search_data(list_t ***buf, ms_expression_t *expre,
    int(get_opera)(char *), int *was_here)
{
    int ope = 0;
    int res = 0;
    ms_expre_word_t *data;

    data = (**buf)->data;
    if (data->parenth == expre->max_depth && (**buf)->next) {
        res = search_op(buf, get_opera, &ope, expre);
        if (res == 1)
            return 1;
        if (res == 2)
            return 2;
        return calculate(buf, expre, ope, was_here);
    }
    return 0;
}

int do_math(ms_expression_t *expre, int(get_opera)(char *))
{
    list_t **buf = &expre->expre_list;
    int was_here = 0;
    int res = 0;

    while (*buf) {
        res = search_data(&buf, expre, get_opera, &was_here);
        if (res == 1)
            continue;
        if (res == 84) {
            dprintf(1, "if: Expression Syntax.\n");
            return 84;
        }
        if (res == 42)
            return 84;
        if (res == 2)
            return 0;
        if (was_here) {
            was_here = 0;
            continue;
        }
        buf = &(*buf)->next;
    }
    return 0;
}

int do_expre(ms_expression_t *expre)
{
    if (do_math(expre, get_prio))
        return 84;
    if (do_math(expre, get_ope))
        return 84;
    if (check_depth(expre)) {
        dprintf(1, "if: Expression Syntax.\n");
        return 84;
    }
    return 0;
}
