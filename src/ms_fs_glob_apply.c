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

static void insert_matches(list_t **tokens,
    list_t *node, char **matches)
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

static int has_no_match(char **matches)
{
    if (!matches)
        return 1;
    if (!matches[0])
        return 1;
    return 0;
}

static int handle_glob(list_t *cur,
    ms_token_t *tok, glob_ctx_t *gctx)
{
    char **matches = glob_expand(tok->word_value, gctx->ctx);

    if (has_no_match(matches)) {
        free_str_arr(matches);
        return glob_error(gctx);
    }
    insert_matches(gctx->tokens, cur, matches);
    ll_remove_node(gctx->tokens, cur);
    free_token(tok);
    free_str_arr(matches);
    return 0;
}

static void init_gctx(glob_ctx_t *gctx,
    list_t **tokens, ms_shell_context_t *ctx)
{
    gctx->tokens = tokens;
    gctx->ctx = ctx;
    gctx->cmd = NULL;
    if (*tokens)
        gctx->cmd = ((ms_token_t *)(*tokens)->data)->word_value;
}

static int apply_token(list_t *cur, glob_ctx_t *gctx)
{
    ms_token_t *tok = cur->data;

    if (!should_glob(tok))
        return 0;
    return handle_glob(cur, tok, gctx);
}

int apply_globbing(list_t **tokens,
    ms_shell_context_t *ctx)
{
    list_t *cur = *tokens;
    list_t *next;
    glob_ctx_t gctx;

    if (!cur)
        return 0;
    init_gctx(&gctx, tokens, ctx);
    if (my_strcmp("if", gctx.cmd) == 0)
        return 0;
    while (cur) {
        next = cur->next;
        if (apply_token(cur, &gctx))
            return 1;
        cur = next;
    }
    return 0;
}
