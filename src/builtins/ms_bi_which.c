/*
** EPITECH PROJECT, 2025
** 42sh
** File description:
** ms_new_builtin.c
*/

#include "minishell1.h"
#include "minishell2.h"
#include "shell.h"
#include "ms_builtins.h"
#include <string.h>
#include <stdio.h>
#include <dirent.h>

int is_builtin(ms_shell_context_t *context, char *args)
{
    for (int i = 0; ms_builtins_list[i].name; i++) {
        if (!strcmp(ms_builtins_list[i].name, args)) {
            return 1;
        }
    }
    return 0;
}

static void check_bin_folder(int *nb, char *str, int i, char **path)
{
    DIR *dir = opendir(path[i]);
    struct dirent *entry;

    if (!dir)
        return;
    for (entry = readdir(dir); entry != NULL; entry = readdir(dir)) {
        if (!strcmp(entry->d_name, str)) {
            *nb = 1;
            closedir(dir);
            printf("%s/%s\n", path[i], entry->d_name);
            return;
        }
    }
    closedir(dir);
}

int is_in_path(char **path, char *str)
{
    int nb = 0;

    for (int i = 0; path[i]; ++i) {
        check_bin_folder(&nb, str, i, path);
        if (nb == 1)
            break;
    }
    return 0;
}

int ms_bi_which(ms_shell_context_t *context, char **args)
{
    char **path = my_explode(km_get(MS_VAR_PATH, context->variables), " \t:");

    if (!path)
        return 1;
    for (int i = 0; args[i]; ++i) {
        if (is_builtin(context, args[i])) {
            dprintf(2, "%s: shell built-in command.\n", args[i]);
            continue;
        }
        is_in_path(path, args[i]);
    }
    free_str_arr(path);
    return 0;
}
