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

static int search_data(list_t ***buf, ms_expression_t *expre,
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

static int can_continue(int *was_here)
{
    if (*was_here) {
        *was_here = 0;
        return 1;
    }
    return 0;
}

static int do_math(ms_expression_t *expre, int(get_opera)(char *))
{
    list_t **buf = &expre->expre_list;
    int was_here = 0;

    for (int res = 0; *buf;) {
        res = search_data(&buf, expre, get_opera, &was_here);
        switch (res) {
            case 1:
                continue;
            case 2:
                return 0;
            case 84:
                dprintf(1, "if: Expression Syntax.\n");
            case 42:
                return 84;
        }
        if (can_continue(&was_here))
            continue;
        buf = &(*buf)->next;
    }
    return 0;
}

int expre_compare(int comp, long val1, long val2)
{
    if (comp == 1)
        return val1 == val2;
    if (comp == 2)
        return val1 != val2;
    return -1;
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
