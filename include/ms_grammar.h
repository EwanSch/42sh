/*
** EPITECH PROJECT, 2025
** Linked List Libs
** File description:
** Include file for linked list functions
** Author:
** Amélie Ambleton--Guth
*/

#ifndef MS_GRAMMAR_H
    #define MS_GRAMMAR_H

    #include "minishell2.h"

static bool parse_sequence(ms_grammar_parser_t *grammar,
    ms_syntax_tree_t *root);
static int visit_sequence(ms_syntax_tree_t *node, ms_shell_context_t *context);

#endif
