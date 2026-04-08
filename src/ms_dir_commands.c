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
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "shell.h"
#include "minishell1.h"
#include <sys/stat.h>

static void fork_callback(char *exec, char **args, char **env)
{
    struct stat statres = {0};

    args[0] = exec;
    if (stat(args[0], &statres) == -1 && errno == ENOENT) {
        my_dprintf(2, "%s: Command not found.\n", args[0]);
        exit(1);
    }
    execve(args[0], args, env);
    my_dprintf(2, "%s: %s.", args[0], strerror(errno));
    if (errno == ENOEXEC)
        my_fputs(2, " Binary file not executable.");
    my_fputs(2, "\n");
    exit(1);
}

static int signal_error_message(int signal)
{
    char coredumped = WCOREDUMP(signal);
    int sigcode = WTERMSIG(signal);

    switch (sigcode) {
        case SIGSEGV:
            my_fputs(2, MYSH_MSG_SEGFAULT);
            break;
        case SIGFPE:
            my_fputs(2, MYSH_MSG_FLTPTEXCEP);
            break;
        case SIGABRT:
            my_fputs(2, MYSH_MSG_ABORTED);
            break;
        default:
            my_dprintf(2, MYSH_MSG_SIGOTHER, sigcode);
    }
    if (coredumped)
        my_fputs(2, MYSH_MSG_COREDUMP);
    my_fputs(2, "\n");
    return sigcode | 0x80;
}

int run_other(char **args, ms_shell_context_t *context)
{
    int status = 0;
    char *full_path = (char[PATH_MAX]) {0};
    char **env_dump;
    __pid_t npid;

    if (my_strchr(args[0], '/')) {
        full_path = args[0];
    } else if (!get_cmd_path(context, args[0], full_path)) {
        my_dprintf(2, "%s: Command not found.\n", args[0]);
        return 1;
    }
    env_dump = ms_dump_env(context);
    npid = fork();
    if (npid == 0)
        fork_callback(full_path, args, env_dump);
    waitpid(npid, &status, 0);
    free_str_arr(env_dump);
    if (WIFSIGNALED(status))
        return signal_error_message(status);
    return WEXITSTATUS(status);
}

static int verify_exit_validity(char **args, int *status)
{
    *status = 0;
    if (args[0] != NULL && args[1] != NULL && args[2] != NULL) {
        my_fputs(2, "exit: Expression Syntax.\n");
        return 1;
    }
    if (!args[1] || my_strlen(args[1]) == 0)
        return 0;
    if (!my_str_isnumerical(args[1])) {
        my_dprintf(2, "exit: %s.\n", my_isnumerical(args[1][0]) ?
            "Badly formed number" : "Expression Syntax");
        return 1;
    }
    *status = my_getexit(args[1]);
    return 0;
}

int run_exit(char **args, ms_shell_context_t *context)
{
    int status;

    if (verify_exit_validity(args, &status))
        return 1;
    free_str_arr(args);
    ms_teardown(context);
    exit(status);
}

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

int run_cd(char **args, ms_shell_context_t *context)
{
    char *expr = cd_destination(args + 1, context);
    int status = 0;
    char *cwd_buffer = getcwd(NULL, 0);

    if (!expr) {
        safe_free(&expr);
        return 1;
    }
    safe_free(&context->last_working_dir);
    context->last_working_dir = cwd_buffer;
    status = chdir(expr);
    if (status != 0)
        my_dprintf(2, "%s: %s.\n", expr, strerror(errno));
    cwd_buffer = getcwd(NULL, 0);
    km_set(MYSH_CWD_ENV, cwd_buffer, &context->env);
    km_set(MS_VAR_CWD, cwd_buffer, &context->variables);
    safe_free(&cwd_buffer);
    safe_free(&expr);
    return status != 0;
}
