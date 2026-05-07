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
#include <string.h>

#include "minishell2.h"
#include "benjalib.h"

static void complete_token(list_t **head, list_t *temp)
{
    while (*head) {
        if (((ms_token_t *) (*head)->data)->type == MS_TOKEN_EOF) {
            *head = temp;
            return;
        }
        head = &(*head)->next;
    }
}

bool is_alias(ms_grammar_parser_t *grammar, char **exist, bool *err)
{
    alias_t **buf = &grammar->ctx_ref->alias;
    list_t **head = &grammar->tokens;
    list_t *temp;
    ms_token_t *tok = NULL;

    while (*buf) {
        tok = grammar->tokens->data;
        if (my_strcmp(tok->type == MS_TOKEN_WORD ?
                tok->word_value : NULL, (*buf)->alias)) {
            buf = &(*buf)->next;
            continue;
        }
        if (*exist && *exist == (*buf)->alias) {
            dprintf(2, "Alias loop.\n");
            *err = true;
            return 0;
        }
        if (!*exist)
            *exist = (*buf)->alias;

        gr_consume(grammar);
        temp = *head;
        *head = cut_words((*buf)->name, grammar->ctx_ref);
        complete_token(head, temp);
        return 1;
    }
    return 0;
}

static pid_t forked_simple_command(ms_syntax_tree_t *node,
    ms_shell_context_t *context, int fdin, int fdout)
{
    pid_t pid;
    int visit_status;

    if (!node || !context)
        return -1;
    if (fdout == STDOUT_FILENO)
        return visit_simple_command(node, context, fdin, fdout);
    pid = fork();
    if (pid == 0) {
        visit_status = visit_simple_command(node, context, fdin, fdout);
        close(fdin);
        close(fdout);
        exit(visit_status);
    }
    free_ast(node);
    return pid;
}

static int build_pipeline(ms_syntax_tree_t *node, ms_shell_context_t *context,
    int *lastin, pid_t *pid)
{
    ms_syntax_tree_t *child;
    int piped[2] = {0};
    bool pipe_status = true;

    for (int index = 0; node->children; index++) {
        child = ll_shift(&node->children);
        if (node->children)
            pipe_status = pipe(piped) == 0;
        else
            piped[1] = STDOUT_FILENO;
        if (!pipe_status)
            return MYSH_ERROR;
        pid[index] = forked_simple_command(child, context, *lastin, piped[1]);
        if (*lastin != STDIN_FILENO)
            close(*lastin);
        if (piped[1] != STDOUT_FILENO)
            close(piped[1]);
        *lastin = piped[0];
    }
    return MYSH_SUCCESS;
}

static int wait_and_compute_status(pid_t pid, int current_status, bool last)
{
    int status = 0;

    if (!last) {
        waitpid(pid, &status, 0);
        if (status) {
            return WIFEXITED(status) ? WEXITSTATUS(status) : WTERMSIG(status);
        }
    } else if (pid != 0)
        return pid;
    return current_status;
}

int pipeline_handler(ms_syntax_tree_t *node, ms_shell_context_t *context)
{
    int status = 0;
    int final_status = 0;
    int lastin = STDIN_FILENO;
    int nb_nodes = ll_size(node->children);
    pid_t pid[nb_nodes];

    status = build_pipeline(node, context, &lastin, pid);
    if (status)
        return error("Pipeline error: %s\n", strerror(status));
    for (int index = 0; index < nb_nodes; index++) {
        final_status = wait_and_compute_status(pid[index],
            final_status, index == nb_nodes - 1);
    }
    return final_status;
}
