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

static int ignore_no_match(char *cmd)
{
    if (!cmd)
        return 0;
    if (my_strcmp(cmd, "echo") == 0)
        return 1;
    if (my_strcmp(cmd, "glob") == 0)
        return 1;
    return 0;
}

static int glob_error(glob_ctx_t *gctx)
{
    if (ignore_no_match(gctx->cmd))
        return 0;
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

static int handle_no_match(char **matches, glob_ctx_t *gctx)
{
    if (!matches)
        return glob_error(gctx);
    if (!matches[0]) {
        free_str_arr(matches);
        return glob_error(gctx);
    }
    return 0;
}

static int handle_glob(list_t *cur, ms_token_t *tok, glob_ctx_t *gctx)
{
    char **matches = glob_expand(tok->word_value, gctx->ctx);

    if (handle_no_match(matches, gctx))
        return 1;
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

int apply_globbing(list_t **tokens, ms_shell_context_t *ctx)
{
    list_t *cur = *tokens;
    list_t *next;
    glob_ctx_t gctx;

    init_gctx(&gctx, tokens, ctx);
    while (cur) {
        next = cur->next;
        if (apply_token(cur, &gctx))
            return 1;
        cur = next;
    }
    return 0;
}
