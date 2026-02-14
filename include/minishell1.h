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

    #define MYSH_HOME_ENV "HOME"

    #include <termios.h>
    #include "benjalib.h"

    #define MYSH_HOME_ENV "HOME"
    #define MYSH_CWD_ENV "PWD"

    #define MYSH_MSG_ABORTED "Aborted"
    #define MYSH_MSG_SEGFAULT "Segmentation fault"
    #define MYSH_MSG_FLTPTEXCEP "Floating point exception"
    #define MYSH_MSG_SIGOTHER "Process terminated by signal %d"
    #define MYSH_MSG_COREDUMP " (core dumped)"

    #define MYSH_HISTORY_SIZE 50
    #define MYSH_IN_BUF_EXTENSION_STEP 256
    #define MYSH_ESC_SEQ_MAXLEN 64

typedef struct ms_shell_context_s ms_shell_context_t;
typedef struct ms_env_entry_s ms_env_entry_t;
typedef struct ms_line_editor_s ms_line_editor_t;
typedef struct ms_editor_command_s ms_editor_command_t;

struct ms_env_entry_s {
    char *key;
    char *value;
};

struct ms_shell_context_s {
    unsigned char last_exit_status;
    char *history[50];
    int history_index;
    list_t *env;
    char *last_working_dir;
};

struct ms_line_editor_s {
    size_t text_len;
    size_t caret_pos;
    char *input_buffer;
    size_t bufsize;
    char *history[MYSH_HISTORY_SIZE];
    int history_top;
    int history_index;
};

struct ms_editor_command_s {
    char key;
    void (*callback)(ms_line_editor_t *, int, int);
};

// Main
void ms_teardown(ms_shell_context_t *context);
void process_line(ms_shell_context_t *context, char *line);

// Path Utility
int get_cmd_path(ms_shell_context_t *context, char *search, char *full_path);

// Explode utility
char **my_explode(char const *str, char const *separator);
void free_str_arr(char **str_arr);
char *my_join(char const *separator, char **str_arr);
int count_words(char const *str, char const *separator);

// Miscellaneous Utils
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

// Editor Commands
void msle_history_up(ms_line_editor_t *lined, int mod, int payload);
void msle_history_down(ms_line_editor_t *lined, int mod, int payload);
void msle_move_left(ms_line_editor_t *lined, int mod, int payload);
void msle_move_right(ms_line_editor_t *lined, int mod, int payload);
void msle_move_start(ms_line_editor_t *lined, int mod, int payload);
void msle_move_end(ms_line_editor_t *lined, int mod, int payload);
void msle_misc_codepoint(ms_line_editor_t *lined, int mod, int payload);

// Line Editor Keybinds
void msle_hit_enter(ms_shell_context_t *context, ms_line_editor_t *lined);
void msle_hit_backspace(ms_line_editor_t *lined);
void msle_hit_tab(ms_line_editor_t *lined);

// Line Editor Tools
void msle_refresh_cursor_position(ms_line_editor_t *lined);
void display_prompt(ms_shell_context_t *context, ms_line_editor_t *lineed);
int msle_extend_input_buffer(ms_line_editor_t *lined);
void msle_add_character(ms_line_editor_t *lined, char c);
int msle_special_key(ms_shell_context_t *ctx, ms_line_editor_t *lined, char c);

// Line Editor Escape Sequences

void msle_read_full_escape(char *sequence, int seq_size, int *i);
void msle_escape_seq_action(ms_line_editor_t *lined, char *seq, int mod);
int msle_get_modifier_key(char *sequence);
void msle_escape_sequence(ms_line_editor_t *lined);

#endif
