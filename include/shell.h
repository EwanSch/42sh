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
    #define MS_VAR_CWD "cwd"
    #define MS_VAR_IGNOREEOF "ignoreeof"
    #define MS_VAR_ADDSUFFIX "addsuffix"
    #define MS_VAR_ARGV "argv"

    #define MS_ENV_TERM "TERM"

    #define ERROR_IGNOREEOF_CTRL_D "Use \"exit\" to leave tcsh."

void set_cwd_variable(ms_shell_context_t *context);
void set_term_variable(ms_shell_context_t *context);
int command_eof(char c, ms_shell_context_t *context);

void save_argv(ms_shell_context_t *context, char **argv, int argc);

#endif
