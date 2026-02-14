/*
** EPITECH PROJECT, 2026
** minishell1
** File description:
** Utility seeking an executable
** in PATH dirs
** Author:
** Amélie Ambleton--Guth
*/

#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>

#include "minishell1.h"

int is_file_in_dir(char *dir, char *search)
{
    DIR *dirp = opendir(dir);
    struct dirent *dp;
    int found = 0;

    if (!dirp)
        return 0;
    while (1) {
        dp = readdir(dirp);
        if (!dp)
            break;
        if (!my_strcmp(dp->d_name, search)) {
            found = 1;
            break;
        }
    }
    closedir(dirp);
    return found;
}

int get_cmd_path(ms_shell_context_t *context, char *search, char *full_path)
{
    char *path = ms_get_env_value("PATH", context);
    char **path_dirs = my_explode(path, ":");

    if (!path || !path_dirs)
        return 0;
    for (int dir_index = 0; path_dirs[dir_index]; dir_index++) {
        if (!is_file_in_dir(path_dirs[dir_index], search))
            continue;
        my_strcat(full_path, path_dirs[dir_index]);
        my_strcat(full_path, "/");
        my_strcat(full_path, search);
        free_str_arr(path_dirs);
        return 1;
    }
    free_str_arr(path_dirs);
    return 0;
}
