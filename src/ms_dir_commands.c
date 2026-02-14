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

#include "minishell1.h"

static void fork_callback(char *exec, char **args, char **env)
{
    args[0] = exec;
    execve(args[0], args, env);
    my_dprintf(2, "%s: %s.\n", args[0], strerror(errno));
    exit(1);
}

static void signal_error_message(int signal)
{
    char coredumped = WCOREDUMP(signal);
    int sigcode = WTERMSIG(signal);

    switch (sigcode) {
        case SIGSEGV:
            my_fputs(2, "Segmentation fault");
            break;
        case SIGFPE:
            my_fputs(2, "Floating point exception");
            break;
        case SIGABRT:
            my_fputs(2, "Aborted");
            break;
        default:
            my_dprintf(2, "Process terminated by signal %d", sigcode);
    }
    if (coredumped)
        my_fputs(2, " (core dumped)");
    my_fputs(2, "\n");
}

int run_other(char **args, ms_shell_context_t *context)
{
    int status = 0;
    char *full_path = (char[PATH_MAX]) {0};
    char **env_dump;
    __pid_t npid;

    if (args[0][0] == '.' || args[0][0] == '/') {
        full_path = args[0];
    } else if (!get_cmd_path(context, args[0], full_path)) {
        my_dprintf(2, "%s: Command not found.\n", args[0]);
        return -1;
    }
    env_dump = ms_dump_env(context);
    npid = fork();
    if (npid == 0)
        fork_callback(full_path, args, env_dump);
    waitpid(npid, &status, 0);
    free_str_arr(env_dump);
    if (WIFSIGNALED(status))
        signal_error_message(status);
    return status;
}

int run_exit(char **args, ms_shell_context_t *context)
{
    char *expr = my_join(" ", args + 1);
    int status;

    if (!expr || strlen(expr) == 0) {
        status = 0;
    } else {
        if (!my_str_isnumerical(expr)) {
            free(expr);
            my_dprintf(2, "exit: Expression Syntax.\n");
            return 1;
        }
        status = my_getnbr(expr);
    }
    free(expr);
    free_str_arr(args);
    ms_teardown(context);
    exit(status);
}

int run_cd(char **args, ms_shell_context_t *context)
{
    char *expr = my_join(" ", args + 1);
    int status = 0;
    char *cwd_buffer;

    if (!expr || strlen(expr) == 0) {
        free(expr);
        expr = my_strdup(ms_get_env_value(MYSH_HOME_ENV, context));
    }
    if (!my_strcmp("-", expr)) {
        free(expr);
        if (context->last_working_dir == NULL)
            expr = my_strdup("");
        else
            expr = my_strdup(context->last_working_dir);
        // expr = my_strdup(ms_get_env_value("OLDPWD", context));
    }
    cwd_buffer = getcwd(NULL, 0);
    if (context->last_working_dir != NULL)
        free(context->last_working_dir);
    context->last_working_dir = cwd_buffer;
    status = chdir(expr);
    if (status != 0)
        my_dprintf(2, "%s: %s.\n", expr, strerror(errno));
    cwd_buffer = getcwd(NULL, 0);
    ms_set_env_value("PWD", cwd_buffer, context);
    free(cwd_buffer);
    free(expr);
    return status;
}
