/*
** EPITECH PROJECT, 2025
** aaa
** File description:
** aab
*/

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void *my_recalloc(void *str, size_t new_size)
{
    void *new_str = calloc(1, new_size);

    if (!new_str) {
        return NULL;
    }
    memset(new_str, 0, new_size);
    if (!str)
        return new_str;
    strcpy(new_str, str);
    free(str);
    return new_str;
}
