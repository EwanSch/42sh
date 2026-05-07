/*
** EPITECH PROJECT, 2026
** aaa
** File description:
** aab
*/

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "minishell2.h"
#include "benjalib.h"

void ll_free_linked(list_t *link, void(*free_every)(void *data))
{
    void *data = NULL;

    while (link) {
        data = ll_shift(&link);
        if (data)
            free_every(data);
    }
}
