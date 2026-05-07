/*
** EPITECH PROJECT, 2026
** minishell2
** File description:
** MiniShell utils
** Author:
** Amélie Ambleton--Guth
*/


// We want the general grammar to be left-recursive.

// LEXEMES RULES
// A lexeme is defined by any sequence of
// letters or symbols that is neither a space, a tab,
// nor a newline, unless they're escaped or quoted.
// A backslash escapes the next character except when
// enclosed by single quotes.
// Unescaped quotes and backslashes are not included in the lexeme.
// Any unescaped/unquoted token (besides 'word's) also breaks
// the lexeme they're in the middle of, if any, emitting
// a token for the part of the lexeme before it,
// then emitting its own token. A new lexeme begins
// right after (or at the first non-blank following).

// TOKENS RULES - First match stops further matching attempts

// 'EOF' - End of input (no associated lexeme)
// '&&' - literal &&
// '||' - literal ||
// '>>' - literal >>
// '<<' - literal <<
// '&' - literal &
// '|' - literal |
// '>' - literal >
// '<' - literal <
// ';' - literal ;
// '(' - literal (
// ')' - literal )
// WORD - any other lexeme



// GRAMMAR RULES
//   Legend:
//     '?' means the previous token can be omitted
//     '*' matches the token between 0 and unlimited times
//     '+' matches the token between 1 and unlimited times

// root:             'sequence' - top-level node
// redirection:      '< WORD'
// redirection:      '<< WORD'
// redirection:      '> WORD'
// redirection:      '>> WORD'
// sequence:         'sequence ; and_or'
// sequence:         'and_or'
// and_or:           'and_or && pipeline'
// and_or:           'and_or || pipeline'
// and_or:           'pipeline'
// pipeline:         'pipeline | simple_command'
// pipeline:         'simple_command'
// simple_command:   'redirection* command redirection*'
// command:          '( sequence )'
// command:          'WORD+'

// Any remaining token that does not fit in
// the grammar ruleset will trigger an error
// and prevent the execution of the input.

#include <string.h>

#include "minishell2.h"
#include "benjalib.h"
#include "ms_grammar.h"

ms_syntax_tree_t *ast_build(ms_syntax_tree_t *parent, ms_tree_type_t type)
{
    ms_syntax_tree_t *node = my_calloc(1, sizeof(ms_syntax_tree_t));

    if (!node)
        return NULL;
    node->type = type;
    node->children = NULL;
    if (parent) {
        ll_push(&parent->children, node);
        node->ctx_ref = parent->ctx_ref;
        node->root_ref = parent->root_ref;
    } else
        node->root_ref = node;
    return node;
}

static ms_token_t *gr_peek(ms_grammar_parser_t *grammar)
{
    if (gr_at_end(grammar))
        return NULL;
    return grammar->tokens->next->data;
}

static void push_word(ms_grammar_parser_t *grammar, ms_syntax_tree_t *parent)
{
    ms_syntax_tree_t *word_node = ast_build(parent, MS_TREE_WORD);
    ms_token_t *tok;

    if (!word_node)
        return;
    tok = gr_consume(grammar);
    if (tok && tok->word_value)
        ll_push(&word_node->children, tok->word_value);
    safe_free(&tok);
}

static bool parse_redirection(ms_grammar_parser_t *grammar,
    ms_syntax_tree_t *parent)
{
    ms_syntax_tree_t *redir_node;

    if (!(gr_testfor(grammar, MS_TOKEN_GREATER) ||
            gr_testfor(grammar, MS_TOKEN_LESS) ||
            gr_testfor(grammar, MS_TOKEN_DOUBLE_GREATER) ||
            gr_testfor(grammar, MS_TOKEN_DOUBLE_LESS)))
        return false;
    if (!gr_testahead(grammar, MS_TOKEN_WORD))
        return !ms_fail_parse(grammar->ctx_ref, MSE_MISSING_NAME_REDIRECT,
            gr_peek(grammar));
    redir_node = ast_build(parent, MS_TREE_REDIRECTION);
    if (!redir_node)
        return false;
    ll_push(&redir_node->children, gr_consume(grammar));
    push_word(grammar, redir_node);
    return true;
}

static bool parse_command(ms_grammar_parser_t *grammar,
    ms_syntax_tree_t *parent, ms_syntax_tree_t **root)
{
    bool first_word = false;

    if (gr_match(grammar, MS_TOKEN_LEFT_BRACKET, true)) {
        if ((*root))
            return false;
        (*root) = ast_build(parent, MS_TREE_SEQUENCE);
        if (!(*root))
            return false;
        return parse_sequence(grammar, (*root));
    }
    if (!(*root)) {
        first_word = true;
        (*root) = ast_build(parent, MS_TREE_COMMAND);
    }
    if (!(*root))
        return false;
    if (first_word)
        while (is_alias(grammar));
    push_word(grammar, (*root));
    return true;
}

static bool parse_simple_command(ms_grammar_parser_t *grammar,
    ms_syntax_tree_t *parent)
{
    char *buf = NULL;
    ms_syntax_tree_t *root = NULL;
    bool all_good = true;

    if (gr_at_end(grammar))
        return true;
    while (is_alias(grammar));
    while (1) {
        if (parse_redirection(grammar, parent))
            continue;
        if (!gr_testfor(grammar, MS_TOKEN_WORD) &&
            !gr_testfor(grammar, MS_TOKEN_LEFT_BRACKET))
            break;
        all_good = parse_command(grammar, parent, &root);
    }
    return all_good;
}

static bool parse_pipeline(ms_grammar_parser_t *grammar,
    ms_syntax_tree_t *parent)
{
    ms_syntax_tree_t *node;
    bool success = true;

    if (gr_at_end(grammar))
        return true;
    while (success) {
        node = ast_build(parent, MS_TREE_SIMPLE_COMMAND);
        if (!node)
            break;
        success = parse_simple_command(grammar, node);
        if (!gr_match(grammar, MS_TOKEN_PIPE, true))
            break;
    }
    return success && verify_pipeline(parent);
}

static bool parse_and_or(ms_grammar_parser_t *grammar, ms_syntax_tree_t *parent)
{
    ms_syntax_tree_t *node;
    bool valid = true;

    if (gr_at_end(grammar))
        return true;
    while (valid) {
        node = ast_build(parent, MS_TREE_PIPELINE);
        if (!node)
            break;
        valid = parse_pipeline(grammar, node);
        if (gr_testfor(grammar, MS_TOKEN_AND) ||
            gr_testfor(grammar, MS_TOKEN_OR))
            ll_push(&parent->children, gr_consume(grammar));
        else
            break;
    }
    return valid;
}

static bool parse_sequence(ms_grammar_parser_t *grammar, ms_syntax_tree_t *root)
{
    ms_syntax_tree_t *node;
    bool valid = true;

    while (!gr_at_end(grammar) && valid) {
        if (gr_match(grammar, MS_TOKEN_SEMICOLON, true))
            continue;
        node = ast_build(root, MS_TREE_AND_OR);
        if (!node)
            break;
        valid = parse_and_or(grammar, node);
        if (gr_match(grammar, MS_TOKEN_RIGHT_BRACKET, true))
            break;
        if (!gr_match(grammar, MS_TOKEN_SEMICOLON, true) &&
            !gr_at_end(grammar)) {
            grammar->errored = true;
            break;
        }
    }
    return valid;
}

ms_syntax_tree_t *ms_generate_ast(list_t *tokens, ms_shell_context_t *context)
{
    ms_grammar_parser_t grammar = {0};
    ms_syntax_tree_t *root = ast_build(NULL, MS_TREE_SEQUENCE);

    if (!root)
        return NULL;
    grammar.ctx_ref = context;
    root->ctx_ref = context;
    grammar.tokens = tokens;
    grammar.errored |= !parse_sequence(&grammar, root);
    free_grammar(&grammar);
    if (grammar.errored) {
        context->last_exit_status = MYSH_ERROR;
        if (!grammar.ctx_ref->is_interactive)
            my_dprintf(STDERR_FILENO, "-- Terminating.\n");
        return NULL;
    }
    return root;
}
