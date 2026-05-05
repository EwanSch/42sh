/*
** EPITECH PROJECT, 2026
** minishell1
** File description:
** Project's entry point
** Auto-Generated with Episetup by Amélie
** Author:
** Amélie Ambleton--Guth
** Feb 2026
*/

#include "ms_builtins.h"

const ms_builtin_t ms_builtins_list[] = {
    { "cd", &ms_bi_cd },
    { "env", &ms_bi_env },
    { "setenv", &ms_bi_setenv },
    { "unsetenv", &ms_bi_unsetenv },
    { "exit", &ms_bi_exit },
    {"alias", &ms_bi_alias},
    {"unalias", &ms_bi_unalias},
    {"which", &ms_bi_which},
    {"where", &ms_bi_where},
    {":", &ms_bi_colon},
    {"chdir", &ms_bi_cd},
    {"set", &ms_bi_set},
    { NULL, NULL }
};
