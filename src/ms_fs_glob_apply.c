/*
** EPITECH PROJECT, 2026
** G-PSU-200-STG-2-1-42sh-1
** File description:
** ms_fs_glob_apply
*/

#include "globbing.h"

static int should_glob(ms_token_t *tok)
{
    if (tok->type != MS_TOKEN_WORD)
        return 0;
    if (!is_glob_pattern(tok->word_value))
        return 0;
    return 1;
}

static int glob_error(glob_ctx_t *gctx)
{
    my_dprintf(2, "%s: No match.\n", gctx->cmd);
    gctx->ctx->last_exit_status = 1;
    return 1;
}

static void insert_matches(list_t **tokens, list_t *node, char **matches)
{
    ms_token_t *new;
    int i = 0;

    while (matches[i]) {
        new = malloc(sizeof(ms_token_t));
        if (!new)
            return;
        new->type = MS_TOKEN_WORD;
        new->word_value = my_strdup(matches[i]);
        ll_insert_before(tokens, node, new);
        i++;
    }
}

static int handle_glob(list_t *cur, ms_token_t *tok, glob_ctx_t *gctx)
{
    char **matches = glob_expand(tok->word_value, gctx->ctx);

    if (!matches || !matches[0])
        return glob_error(gctx);
    insert_matches(gctx->tokens, cur, matches);
    ll_remove_node(gctx->tokens, cur);
    free_token(tok);
    free_str_arr(matches);
    return 0;
}

int apply_globbing(list_t **tokens, ms_shell_context_t *ctx)
{
    list_t *cur = *tokens;
    list_t *next;
    ms_token_t *tok;
    glob_ctx_t gctx;

    gctx.tokens = tokens;
    gctx.ctx = ctx;
    gctx.cmd = NULL;
    while (cur) {
        next = cur->next;
        tok = cur->data;
        if (should_glob(tok) && handle_glob(cur, tok, &gctx))
            return 1;
        gctx.cmd = tok->word_value;
        cur = next;
    }
    return 0;
}
