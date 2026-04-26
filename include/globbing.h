/*
** EPITECH PROJECT, 2026
** G-PSU-200-STG-2-1-42sh-1
** File description:
** globbing
*/

#ifndef GLOBBING_H
    #define GLOBBING_H

    #include <errno.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <threads.h>
    #include <unistd.h>
    #include <linux/limits.h>
    #include <sys/wait.h>
    #include <sys/types.h>
    #include <dirent.h>
    #include "minishell1.h"
    #include "minishell2.h"

int is_glob_pattern(const char *str);
void apply_globbing(list_t **tokens, ms_shell_context_t *ctx);

/* core */
char **glob_expand(char *pattern, ms_shell_context_t *ctx);

/* matching */
int glob_match(const char *pattern, const char *str);

/* brace */
char **brace_expand(const char *pattern);

/* match */
int match_range(const char *pattern, char c, int *i);
char **read_dir_match(char *pattern);

/* utils */
char **list_to_array(list_t *lst);
void ll_remove_node(list_t **list, list_t *node);
void ll_insert_after(list_t *node, void *data);
void ll_insert_before(list_t **list, list_t *node, void *data);
#endif
