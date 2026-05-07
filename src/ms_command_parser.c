/*
** EPITECH PROJECT, 2026
** minishell1
** File description:
** MiniShell utils
** Author:
** Amélie Ambleton--Guth
*/

#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <stdbool.h>
#include "minishell2.h"
#include "var_substitution.h"

int end_parse_err(ms_parser_t *parser)
{
    if (parser->quote_mode == MS_QUOTE_SINGLE)
        return ms_error("Unmatched '\''.");
    if (parser->quote_mode == MS_QUOTE_DOUBLE)
        return ms_error("Unmatched '\"'.");
    return 0;
}

static int is_solo_word(char const *string)
{
    if (!string || !string[0])
        return 0;
    if (string[0] == string[1] && my_strchr("&|<>", *string))
        return 2;
    if (my_strchr("&;|<>()", *string))
        return 0;       // HERE WAS ORIGINALLY 1
    return 0;
}

static ms_token_type_t get_solo_token_type(char *chr)
{
    char const *ref = ".|;&<>()";

    if (!chr || !chr[0] || !is_solo_word(chr))
        return MS_TOKEN_WORD;
    if (my_strncmp(chr, ">>", 2) == 0)
        return MS_TOKEN_DOUBLE_GREATER;
    if (my_strncmp(chr, "<<", 2) == 0)
        return MS_TOKEN_DOUBLE_LESS;
    if (my_strncmp(chr, "&&", 2) == 0)
        return MS_TOKEN_AND;
    if (my_strncmp(chr, "||", 2) == 0)
        return MS_TOKEN_OR;
    return my_strindexof(ref, *chr);
}

static char *sanitize_lexeme(char *lexeme, int max)
{
    char *duplicated;
    int length = 0;

    if (!lexeme)
        return NULL;
    for (int i = 0; i < max; i++) {
        if (lexeme[i] != RECORD_SEPARATOR)
            length++;
    }
    duplicated = malloc(length + 1);
    if (!duplicated)
        return NULL;
    duplicated[length] = '\0';
    for (int i = max - 1; i >= 0; i--)
        if (lexeme[i] != RECORD_SEPARATOR) {
            --length;
            duplicated[length] = lexeme[i];
        }
    return duplicated;
}

static void add_token(list_t **lst, char *lexeme, int length)
{
    ms_token_t *token;

    token = malloc(sizeof(ms_token_t));
    if (!token)
        return;
    if (!lexeme) {
        token->word_value = NULL;
        token->type = MS_TOKEN_EOF;
    } else if (is_solo_word(lexeme)) {
        token->word_value = NULL;
        token->type = get_solo_token_type(lexeme);
    } else {
        token->word_value = sanitize_lexeme(lexeme, length);
        token->type = MS_TOKEN_WORD;
    }
    ll_push(lst, token);
}

int which_quote(ms_parser_t *parser, char *string, int i)
{
    parser->backslashed = string[i] == '\\';
    if (string[i] == '\'')
        parser->quote_mode = MS_QUOTE_SINGLE;
    if (string[i] == '\"')
        parser->quote_mode = MS_QUOTE_DOUBLE;
    if (PARSER_ESCAPING(parser)) {
        string[i] = RECORD_SEPARATOR;
        return 1;
    }
    return 0;
}

int update_parser2(char *string, int i, ms_parser_t *parser)
{
    if (parser->backslashed) {
        parser->backslashed = false;
        return 1;
    }
    if ((string[i] == '\'' && (parser->quote_mode == MS_QUOTE_SINGLE)) ||
        string[i] == '\"' && (parser->quote_mode == MS_QUOTE_DOUBLE)) {
        parser->quote_mode = MS_QUOTE_NONE;
        string[i] = RECORD_SEPARATOR;
        return 1;
    }
    if (PARSER_ESCAPING(parser))
        return 1;
    if (which_quote(parser, string, i))
        return 1;
    return 0;
}

int delimit_words(char *string, list_t **lst, ms_parser_t *parser)
{
    int skip = is_solo_word(string);
    int i = 0;

    if (skip && !PARSER_ESCAPING(parser)) {
        add_token(lst, string, skip);
        return skip;
    }
    for (; string[i]; i++) {
        if (update_parser2(string, i, parser))
            continue;
        if (IS_SEPARATOR(string[i]) || is_solo_word(string + i))
            break;
    }
    add_token(lst, string, i);
    return i;
}

list_t *cut_words(char *string, ms_shell_context_t *context)
{
    list_t *lst = NULL;
    ms_parser_t parser = {0};
    char *var_string = var_sub(string, context);

    if (mismatch(string) || !var_string) {
        if (var_string)
            free(var_string);
        context->last_exit_status = 1;
        safe_free(&var_string);
        return NULL;
    }
    for (int i = 0; var_string[i]; i++) {
        parser.curr_char = i;
        if (IS_SEPARATOR(var_string[i]) && !PARSER_ESCAPING(&parser))
            continue;
        i += delimit_words(var_string + i, &lst, &parser) - 1;
    }
    safe_free(&var_string);
    add_token(&lst, NULL, 0);
    return lst;
}
