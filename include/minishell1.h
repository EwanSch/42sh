/*
** EPITECH PROJECT, 2025
** Linked List Libs
** File description:
** Include file for linked list functions
** Author:
** Amélie Ambleton--Guth
*/

#ifndef MINISHELL1_H
    #define MINISHELL1_H

    #define MYSH_OLDPWD_ENV "42SH_OLDPWD"
    #define MYSH_HOME_ENV "HOME"

    #include "my.h"
    #include "linked_list.h"
    #include "my_printf.h"
    #include <termios.h>

typedef struct ms_shell_context_s ms_shell_context_t;
typedef struct ms_env_entry_s ms_env_entry_t;

struct ms_env_entry_s {
    char *key;
    char *value;
};

struct ms_shell_context_s {
    unsigned char last_exit_status;
    char *history[50];
    int history_index;
    list_t *env;
};

// Main
void ms_teardown(ms_shell_context_t *context);

// Path Utility
int get_cmd_path(ms_shell_context_t *context, char *search, char *full_path);

// Explode utility
char **my_explode(char const *str, char const *separator);
void free_str_arr(char **str_arr);
char *my_join(char const *separator, char **str_arr);
int count_words(char const *str, char const *separator);

// Miscellaneous Utils
void *my_calloc(size_t count, size_t unit_size);
int my_str_isnumerical(char const *str);

// Env utils
void ms_populate_env_from_dump(char **env_dump, ms_shell_context_t *context);
char **ms_dump_env(ms_shell_context_t *context);
void ms_set_env_value(char *key, char *value, ms_shell_context_t *context);
char *ms_get_env_value(char *key, ms_shell_context_t *context);
void ms_unset_env_value(char *key, ms_shell_context_t *context);

// Env commands
int ms_env_setenv(char **args, ms_shell_context_t *context);
int ms_env_show(char **args, ms_shell_context_t *context);
int ms_env_unset(char **args, ms_shell_context_t *context);

// Workdir commands
int run_cd(char **args, ms_shell_context_t *context);
int run_exit(char **args, ms_shell_context_t *context);
int run_other(char **args, ms_shell_context_t *context);

// Reading utils
void enable_raw_mode(struct termios *orig_termios);
void disable_raw_mode(struct termios *orig_termios);

#endif
