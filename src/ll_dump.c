/*
** EPITECH PROJECT, 2026
** aaa
** File description:
** aab
*/

#include <stdlib.h>

#include "minishell2.h"
#include "benjalib.h"

void ll_dump(list_t *link, void(*print)(list_t *link))
{
    list_t *buf = link;

    while (buf) {
        print(buf);
        buf = buf->next;
    }
}
