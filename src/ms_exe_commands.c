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
