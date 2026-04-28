/*
** EPITECH PROJECT, 2026
** aaa
** File description:
** aab
*/

#include <string.h>

#include "minishell1.h"
#include "minishell2.h"
#include "ms_builtins.h"

int is_space(char *name)
{
    for (int i = 0; name[i]; i++) {
        if (name[i] == ' ')
            return 1;
    }
    return 0;
}

void dump_all_alias(alias_t *alias)
{
    alias_t *buf = alias;
    int space = 0;

    while (buf) {
        space = is_space(buf->name);
        printf("%s \t%s%s%s\n", buf->alias,
            !space ? "" : "(", buf->name, !space ? "" : ")");
        buf = buf->next;
    }
}

void show_alias(char *args, alias_t *alias)
{
    alias_t *buf = alias;

    while (buf) {
        if (!strcmp(args, buf->alias))
            printf("%s\n", buf->name);
        buf = buf->next;
    }
}

int insert_alias(char **args, alias_t **alias)
{
    alias_t *new_node = malloc(sizeof(alias_t));
    list_t *list = {0};
    char *str;

    if (!new_node)
        return 84;
    new_node->alias = my_strdup(args[0]);
    for (int i = 1; args[i]; i++) {
        ll_push(&list, args[i]);
    }
    str = ll_to_str(&list, " ");
    new_node->name = str;
    new_node->next = *alias;
    *alias = new_node;
    return 0;
}

int ms_bi_alias(ms_shell_context_t *context, char **argv)
{
    int argc = 0;

    for (char **arg = argv; arg[argc]; argc++);
    if (argc == 0) {
        dump_all_alias(context->alias);
        return 0;
    }
    if (argc == 1) {
        show_alias(*argv, context->alias);
        return 0;
    }
    if (insert_alias(argv, &context->alias))
        return 84;
    return 0;
}
