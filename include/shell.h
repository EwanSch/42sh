/*
** EPITECH PROJECT, 2025
** Linked List Libs
** File description:
** Include file for linked list functions
** Author:
** Amélie Ambleton--Guth
*/

#ifndef SHELL_H
    #define SHELL_H

    #include "minishell1.h"

    #define MS_VAR_TERM "term"
    #define MS_VAR_PRECMD "precmd"
    #define MS_VAR_CWDCMD "cwdcmd"
    #define MS_VAR_CWD "cwd"

    #define MS_ENV_TERM "TERM"

void set_cwd_variable(ms_shell_context_t *context);
void set_term_variable(ms_shell_context_t *context);

#endif