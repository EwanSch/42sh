/*
** EPITECH PROJECT, 2026
** minishell1
** File description:
** MiniShell utils
** Author:
** Amélie Ambleton--Guth
*/

#include <ctype.h>
#include <stddef.h>
#include <stdlib.h>

int my_str_isnumerical(char const *str)
{
    if (!str)
        return 0;
    for (; *str; str++) {
        if ((*str < '0' || *str > '9') && *str != '-')
            return 0;
    }
    return 1;
}
