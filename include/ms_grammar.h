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
    #include <unistd.h>

    #define MS_LEFT_BRACKET_MUCH "Too many ('s.\n"
    #define MS_RIGHT_BRACKET_MUCH "Too many )'s.\n"

static bool parse_sequence(ms_grammar_parser_t *grammar,
    ms_syntax_tree_t *root);
static int visit_sequence(ms_syntax_tree_t *node, ms_shell_context_t *context);
int check_parentheses_basic(const char *line);
#endif
