/*
** EPITECH PROJECT, 2025
** Linked List Libs
** File description:
** Include file for linked list functions
** Author:
** Amélie Ambleton--Guth
*/

#ifndef MS_BUILTINS_H
    #define MS_BUILTINS_H

    #define OPE_EQUAL "=="
    #define OPE_DIFF "!="

    #define OPE_ADD "+"
    #define OPE_SUS "-"
    #define OPE_MULT "*"
    #define OPE_DIV "/"
    #define OPE_MOD "%"

    #include "minishell1.h"

typedef struct {
    char *name;
    int (*callback)(ms_shell_context_t *, char **);
} ms_builtin_t;

extern const ms_builtin_t ms_builtins_list[];

// Implementations
int ms_bi_exit(ms_shell_context_t *context, char **argv);
int ms_bi_cd(ms_shell_context_t *context, char **argv);
int ms_bi_env(ms_shell_context_t *context, char **argv);
int ms_bi_setenv(ms_shell_context_t *context, char **argv);
int ms_bi_unsetenv(ms_shell_context_t *context, char **argv);
int ms_bi_alias(ms_shell_context_t *context, char **argv);
int ms_bi_unalias(ms_shell_context_t *context, char **argv);
int ms_bi_if(ms_shell_context_t *ctx, char **args);
int ms_bi_which(ms_shell_context_t *context, char **argv);
int ms_bi_where(ms_shell_context_t *context, char **argv);
int ms_bi_colon(ms_shell_context_t *context, char **args);
int ms_bi_set(ms_shell_context_t *context, char **args);
int ms_bi_glob(ms_shell_context_t *context, char **args);
int ms_bi_echo(ms_shell_context_t *context, char **args);

#endif
