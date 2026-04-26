/*
** EPITECH PROJECT, 2026
** G-PSU-200-STG-2-1-42sh-1
** File description:
** ms_fs_glob_apply
*/

#include "globbing.h"

static void insert_matches(list_t **tokens, list_t *node, char **matches)
{
    ms_token_t *new;

    for (int i = 0; matches[i]; i++) {
        new = malloc(sizeof(ms_token_t));
        new->type = MS_TOKEN_WORD;
        new->word_value = my_strdup(matches[i]);
        ll_insert_before(tokens, node, new);
    }
}

static void handle_glob(list_t **tokens, list_t *cur,
    ms_token_t *tok, ms_shell_context_t *ctx)
{
    char **matches = glob_expand(tok->word_value, ctx);

    if (!matches || !matches[0]) {
        my_dprintf(2, "%s: No match.\n", tok->word_value);
        return;
    }
    insert_matches(tokens, cur, matches);
    ll_remove_node(tokens, cur);
    free_token(tok);
    free_str_arr(matches);
}

void apply_globbing(list_t **tokens, ms_shell_context_t *ctx)
{
    list_t *cur = *tokens;
    list_t *next;
    ms_token_t *tok;

    while (cur) {
        next = cur->next;
        tok = cur->data;
        if (tok->type == MS_TOKEN_WORD &&
            is_glob_pattern(tok->word_value))
            handle_glob(tokens, cur, tok, ctx);
        cur = next;
    }
}
