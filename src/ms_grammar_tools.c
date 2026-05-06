/*
** EPITECH PROJECT, 2026
** minishell2
** File description:
** MiniShell utils
** Author:
** Amélie Ambleton--Guth
*/

#include "minishell2.h"
#include "benjalib.h"
#include "ms_grammar.h"

static ms_token_type_t gr_current(ms_grammar_parser_t *grammar)
{
    return ((ms_token_t *) (grammar->tokens->data))->type;
}

static ms_token_type_t gr_peek(ms_grammar_parser_t *grammar)
{
    if (gr_at_end(grammar))
        return MS_TOKEN_EOF;
    return ((ms_token_t *) (grammar->tokens->next->data))->type;
}

bool gr_at_end(ms_grammar_parser_t *grammar)
{
    return gr_current(grammar) == MS_TOKEN_EOF;
}

bool gr_testfor(ms_grammar_parser_t *grammar, ms_token_type_t type)
{
    return gr_current(grammar) == type;
}

bool gr_testahead(ms_grammar_parser_t *grammar, ms_token_type_t type)
{
    return gr_peek(grammar) == type;
}

ms_token_t *gr_consume(ms_grammar_parser_t *grammar)
{
    if (gr_at_end(grammar))
        return NULL;
    return ll_shift(&grammar->tokens);
}

ms_token_t *gr_match(ms_grammar_parser_t *grammar, ms_token_type_t type,
    bool destroy)
{
    ms_token_t *token;

    if (!gr_testfor(grammar, type))
        return NULL;
    token = gr_consume(grammar);
    if (destroy)
        free_token(token);
    return token;
}
