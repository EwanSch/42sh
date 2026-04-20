/*
** EPITECH PROJECT, 2025
** Linked List Libs
** File description:
** Include file for linked list functions
** Author:
** Amélie Ambleton--Guth
*/

#ifndef VAR_SUBSTITUTION_H
    #define VAR_SUBSTITUTION_H
    #include <stddef.h>

typedef struct var_utils_s var_utils_t;

struct var_utils_s {
    int word_size;
    int dollar_pos;
    char *str;
};

void *my_recalloc(void *str, size_t new_size);
char *alloc_filled_str(char *str);
char *var_sub(char *str, ms_shell_context_t *ctx);

#endif
