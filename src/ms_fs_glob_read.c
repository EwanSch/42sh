/*
** EPITECH PROJECT, 2026
** G-PSU-200-STG-2-1-42sh-1
** File description:
** ms_fs_glob_read
*/

#include "globbing.h"

static void fill_matches(DIR *dir, char *pattern, list_t **lst)
{
    struct dirent *dp;

    while ((dp = readdir(dir))) {
        if (dp->d_name[0] == '.' && pattern[0] != '.')
            continue;
        if (glob_match(pattern, dp->d_name))
            ll_push(lst, my_strdup(dp->d_name));
    }
}

char **read_dir_match(char *pattern)
{
    DIR *dir = opendir(".");
    list_t *lst = NULL;

    if (!dir)
        return NULL;
    fill_matches(dir, pattern, &lst);
    closedir(dir);
    return list_to_array(lst);
}
