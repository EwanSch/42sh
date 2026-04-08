/*
** EPITECH PROJECT, 2026
** minishell1
** File description:
** MiniShell utils
** Author:
** Amélie Ambleton--Guth
*/

#include <string.h>

char *alloc_filled_str(char *str)
{
    char *res = strdup(str);

    return res;
}
