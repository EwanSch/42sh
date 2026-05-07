/*
** EPITECH PROJECT, 2026
** aaa
** File description:
** aab
*/

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "minishell2.h"
#include "benjalib.h"


int fill_ll(list_t **ll, char *str)
{
    char *data = strdup(str);

    ll_push(ll, data);
    return 0;
}

char *data(void *data)
{
    return data;
}

void free_here(void *data)
{
    free(data);
}

char *array_to_str(char **arr)
{
    list_t *ll = NULL;
    char *str = NULL;

    for (int i = 0; arr[i]; i++) {
        fill_ll(&ll, arr[i]);
    }
    str = ll_to_str(&ll, " ", data);
    ll_free_linked(ll, free_here);
    return str;
}
