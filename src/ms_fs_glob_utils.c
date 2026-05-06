/*
** EPITECH PROJECT, 2026
** G-PSU-200-STG-2-1-42sh-1
** File description:
** ms_fs_glob_utils
*/

#include "globbing.h"

void ll_remove_node(list_t **list, list_t *node)
{
    list_t **cur;

    if (!list || !*list || !node)
        return;
    cur = list;
    while (*cur && *cur != node)
        cur = &(*cur)->next;
    if (!*cur)
        return;
    *cur = node->next;
    free(node);
}

void ll_insert_before(list_t **list, list_t *node, void *data)
{
    list_t **cur;

    if (!list || !node)
        return;
    cur = list;
    while (*cur && *cur != node)
        cur = &(*cur)->next;
    if (!*cur)
        return;
    ll_unshift(cur, data);
}

void ll_insert_after(list_t *node, void *data)
{
    list_t *new;

    if (!node)
        return;
    new = malloc(sizeof(list_t));
    if (!new)
        return;
    new->data = data;
    new->next = node->next;
    node->next = new;
}
