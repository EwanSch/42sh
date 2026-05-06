/*
** EPITECH PROJECT, 2026
** G-PSU-200-STG-2-1-42sh-1
** File description:
** ms_fs_glob_pattern
*/

#include "globbing.h"

int is_glob_pattern(const char *str)
{
    if (!str)
        return 0;
    for (int i = 0; str[i]; i++) {
        if (str[i] == '*' || str[i] == '?' ||
            str[i] == '[' || str[i] == '{')
            return 1;
        if (i == 0 && (str[i] == '~' || str[i] == '^'))
            return 1;
    }
    return 0;
}
