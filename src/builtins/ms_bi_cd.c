/*
** EPITECH PROJECT, 2026
** minishell1
** File description:
** MiniShell translation unit
** containing commands related
** to current dir manipulation
** Author:
** Amélie Ambleton--Guth
** February 2026
*/

#include <errno.h>
#include <string.h>

#include "ms_builtins.h"

static int cd_empty_dest(ms_shell_context_t *context, char **expr)
{
    if (!expr || !context)
        return 1;
    safe_free(expr);
    *expr = my_strdup(km_get_or_default(MYSH_HOME_ENV, context->env, NULL));
    if (!*expr) {
        my_dprintf(2, "cd: No home directory.\n");
        return 1;
    }
    return 0;
}

static char *cd_destination(char **args, ms_shell_context_t *context)
{
    char *expr;

    if (args[0] != NULL && args[1] != NULL) {
        my_dprintf(2, "cd: Too many arguments.\n");
        return NULL;
    }
    expr = my_strdup(args[0]);
    if (!expr || my_strlen(expr) == 0) {
        if (cd_empty_dest(context, &expr))
            return NULL;
        return expr;
    }
    if (!my_strcmp("-", expr)) {
        safe_free(&expr);
        expr = my_strdup(context->last_working_dir);
        if (!expr)
            expr = my_strdup("");
    }
    return expr;
}

int ms_bi_cd(ms_shell_context_t *ctx, char **argv)
{
    char *expr = cd_destination(argv, ctx);
    int status = 0;
    char *cwd_buffer = getcwd(NULL, 0);

    if (!expr) {
        safe_free(&expr);
        return 1;
    }
    safe_free(&ctx->last_working_dir);
    ctx->last_working_dir = cwd_buffer;
    status = chdir(expr);
    if (status != 0)
        my_dprintf(2, "%s: %s.\n", expr, strerror(errno));
    cwd_buffer = getcwd(NULL, 0);
    km_set(MYSH_CWD_ENV, cwd_buffer, &ctx->env);
    safe_free(&cwd_buffer);
    safe_free(&expr);
    return status != 0;
}
