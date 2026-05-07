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

int get_prio(char *ope)
{
    if (!strcmp(ope, OPE_MULT))
        return 3;
    if (!strcmp(ope, OPE_DIV))
        return 4;
    if (!strcmp(ope, OPE_MOD))
        return 5;
    return 0;
}

int get_ope(char *ope)
{
    if (!strcmp(ope, OPE_ADD))
        return 1;
    if (!strcmp(ope, OPE_SUS))
        return 2;
    return -1;
}
