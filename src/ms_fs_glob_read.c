/*
** EPITECH PROJECT, 2026
** G-PSU-200-STG-2-1-42sh-1
** File description:
** ms_fs_glob_read
*/

#include "globbing.h"

static int should_skip(struct dirent *dp, char *pattern)
{
    if (dp->d_name[0] == '.' && pattern[0] != '.')
        return 1;
    return 0;
}

static int is_match(struct dirent *dp, char *pattern)
{
    if (should_skip(dp, pattern))
        return 0;
    if (!glob_match(pattern, dp->d_name))
        return 0;
    return 1;
}

static void fill_matches(DIR *dir, char *pattern, list_t **lst)
{
    struct dirent *dp = readdir(dir);

    while (dp) {
        if (is_match(dp, pattern))
            ll_push(lst, my_strdup(dp->d_name));
        dp = readdir(dir);
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
