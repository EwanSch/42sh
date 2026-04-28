/*
** EPITECH PROJECT, 2026
** minishell2
** File description:
** MiniShell utils
** Author:
** Amélie Ambleton--Guth
*/

#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>

#include "minishell2.h"
#include "benjalib.h"

static int trigger_command(char **args, ms_shell_context_t *context,
    int in, int out)
{
    int old_in = dup(STDIN_FILENO);
    int old_out = dup(STDOUT_FILENO);
    int res = 0;

    dup2(in, STDIN_FILENO);
    dup2(out, STDOUT_FILENO);
    res = run_command(args, context);
    free_str_arr(args);
    if (in != STDIN_FILENO)
        close(in);
    if (out != STDOUT_FILENO)
        close(out);
    dup2(old_in, STDIN_FILENO);
    dup2(old_out, STDOUT_FILENO);
    return res;
}

static int visit_command(ms_syntax_tree_t *node,
    ms_shell_context_t *context, int fdin, int fdout)
{
    char **args;
    ms_syntax_tree_t *word;

    if (!node || !context)
        return MYSH_ERROR;
    if (node->type != MS_TREE_COMMAND)
        return MYSH_ERROR;
    args = my_calloc(ll_size(node->children) + 1, sizeof(char *));
    if (!args)
        return MYSH_ERROR;
    for (int i = 0; node->children; i++) {
        word = ll_shift(&node->children);
        if (word->type != MS_TREE_WORD)
            return MYSH_ERROR;
        args[i] = ll_shift(&word->children);
        free(word);
    }
    free(node);
    return trigger_command(args, context, fdin, fdout);
}

int visit_simple_command(ms_syntax_tree_t *node, ms_shell_context_t *context,
    int fdin, int fdout)
{
    ms_syntax_tree_t *cmd;
    ms_syntax_tree_t *child;
    int redir_status = 0;

    if (!node || !context)
        return MYSH_ERROR;
    if (!node->children || ((ms_syntax_tree_t *) node->children->data)->type
        != MS_TREE_COMMAND)
        return MYSH_ERROR;
    cmd = ll_shift(&node->children);
    while (node->children) {
        child = ll_shift(&node->children);
        redir_status = visit_redirection(child, context, &fdin, &fdout);
        free(child);
        if (redir_status)
            return redir_status;
    }
    free(node);
    return visit_command(cmd, context, fdin, fdout);
}

static int visit_pipeline(ms_syntax_tree_t *node, ms_shell_context_t *context)
{
    int status = 0;

    if (VIS_VALID(node && context, node, MS_TREE_PIPELINE))
        return MYSH_ERROR;
    if (node->children && !node->children->next) {
        status = visit_simple_command(ll_shift(&node->children), context,
            STDIN_FILENO, STDOUT_FILENO);
        free(node);
        return status;
    }
    return pipeline_handler(node, context);
}

static int visit_and_or(ms_syntax_tree_t *node, ms_shell_context_t *context)
{
    ms_syntax_tree_t *child;
    ms_token_t *token;
    int res = 0;

    while (node->children) {
        child = ll_shift(&node->children);
        if (child->type == MS_TREE_PIPELINE)
            res = visit_pipeline(child, context);
        token = ll_shift(&node->children);
        if (!token)
            break;
        if ((token->type == MS_TOKEN_AND && res != 0) ||
            (token->type == MS_TOKEN_OR && res == 0))
            return res;
    }
    return res;
}

static int visit_sequence(ms_syntax_tree_t *node, ms_shell_context_t *context)
{
    ms_syntax_tree_t *child;
    int res = 0;

    context->last_exit_status = 0;
    if (VIS_VALID(node && context, node, MS_TREE_SEQUENCE))
        return 84;
    while (node->children) {
        child = ll_shift(&node->children);
        res = visit_and_or(child, context);
        if (res != 0)
            context->last_exit_status = res;
        safe_free(&child);
    }
    return res;
}

int ms_runner(list_t *tokens, ms_shell_context_t *context)
{
    ms_syntax_tree_t *ast;
    int run_result = 0;

    if (!tokens || !context)
        return MYSH_ERROR;
    ast = ms_generate_ast(tokens, context);
    if (!ast)
        return MYSH_ERROR;
    run_result = visit_sequence(ast, context);
    free_ast(ast);
    return run_result;
}
