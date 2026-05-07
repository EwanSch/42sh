/*
** EPITECH PROJECT, 2026
** minishell2
** File description:
** MiniShell utils
** Author:
** Amélie Ambleton--Guth
*/


#include "minishell2.h"

bool search_alias(ms_syntax_tree_t **root,
    bool first_word, ms_grammar_parser_t *grammar)
{
    char *exist = NULL;
    bool err = false;

    if (!(*root))
        return false;
    if (first_word)
        while (is_alias(grammar, &exist, &err));
    if (err) {
        free_token(gr_consume(grammar));
        return false;
    }
    return true;
}
