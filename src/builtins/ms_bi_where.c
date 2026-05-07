/*
** EPITECH PROJECT, 2025
** 42sh
** File description:
** ms_bi_where.c
*/

#include "minishell1.h"
#include "minishell2.h"
#include "shell.h"
#include <string.h>
#include <stdio.h>
#include <dirent.h>

static void check_bin_folder(char *str, int i, char **path)
{
    DIR *dir = opendir(path[i]);
    struct dirent *entry;

    if (!dir)
        return;
    for (entry = readdir(dir); entry != NULL; entry = readdir(dir)) {
        if (!strcmp(entry->d_name, str))
            printf("%s/%s\n", path[i], entry->d_name);
    }
    closedir(dir);
}

int is_in_path_where(char **path, char *str)
{
    for (int i = 0; path[i]; ++i) {
        check_bin_folder(str, i, path);
    }
    return 0;
}

int ms_bi_where(ms_shell_context_t *context, char **args)
{
    char **path = my_explode(km_get(MS_VAR_PATH, context->variables), " \t:");

    if (!path)
        return 1;
    for (int i = 0; args[i]; ++i) {
        if (is_builtin(context, args[i])) {
            dprintf(2, "%s: shell built-in command.\n", args[i]);
            continue;
        }
        is_in_path_where(path, args[i]);
    }
    free_str_arr(path);
    return 0;
}
