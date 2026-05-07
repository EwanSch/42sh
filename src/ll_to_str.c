/*
** EPITECH PROJECT, 2026
** aaa
** File description:
** aab
*/

#include <stdlib.h>

#include "minishell2.h"
#include "benjalib.h"

char *fill_str(int size, char *in_btw, list_t *buf, char *(find_data)(void *))
{
    int curr = 0;
    char *str;

    str = malloc(sizeof(char) * (size + 1));
    if (!str)
        return NULL;
    str[size] = '\0';
    while (buf) {
        if (curr)
            my_snprintf(str, size + 1, "%s%s%s",
                str, in_btw ? in_btw : "", find_data(buf->data));
        else
            my_strcpy(str, find_data(buf->data));
        curr++;
        buf = buf->next;
    }
    return str;
}

char *ll_to_str(list_t **ll, char *in_btw, char *(find_data)(void *))
{
    list_t *buf = *ll;
    list_t *other_buf = *ll;
    int size = 0;

    if (in_btw)
        size = my_strlen(in_btw) * ll_size(*ll);
    while (buf) {
        size += my_strlen(buf->data);
        buf = buf->next;
    }
    return fill_str(size, in_btw, other_buf, find_data);
}
