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

    #include <stdbool.h>
    #include <termios.h>
    #include "benjalib.h"

    #define MYSH_HOME_ENV "HOME"
    #define MYSH_PATH_ENV "PATH"
    #define MYSH_CWD_ENV "PWD"

    #define MYSH_MSG_ABORTED "Aborted"
    #define MYSH_MSG_SEGFAULT "Segmentation fault"
    #define MYSH_MSG_FLTPTEXCEP "Floating exception"
    #define MYSH_MSG_SIGOTHER "Process terminated by signal %d"
    #define MYSH_MSG_COREDUMP " (core dumped)"

    #define MYSH_ERROR 1
    #define MYSH_SUCCESS 0

    #define MYSH_HISTORY_SIZE 50
    #define MYSH_IN_BUF_EXTENSION_STEP 256
    #define MYSH_ESC_SEQ_MAXLEN 64

    #define MAX_TIME 6
    #define CMD_STRING 64

typedef struct ms_shell_context_s ms_shell_context_t;
typedef struct ms_env_entry_s ms_env_entry_t;
typedef struct km_entry_s km_entry_t;
typedef list_t keymap_t;
typedef struct ms_line_editor_s ms_line_editor_t;
typedef struct ms_editor_command_s ms_editor_command_t;
typedef struct alias_s alias_t;


struct alias_s {
    char *name;
    char *alias;
    struct alias_s *next;
};

struct ms_env_entry_s {
    char *key;
    char *value;
};

struct km_entry_s {
    char *key;
    char *value;
};

typedef struct {
    FILE *stream;
    char *buf;
    size_t size;
    bool open_by_linereader;
    int line;
} linereader_t;

struct ms_shell_context_s {
    char *line_buffer;
    unsigned char last_exit_status;
    char *history[MYSH_HISTORY_SIZE];
    char *time[MYSH_HISTORY_SIZE];
    int history_index;
    char *last_working_dir;
    alias_t *alias;
    keymap_t *variables;
    keymap_t *env;
    linereader_t *reader;
    bool is_interactive;
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
int process_line(ms_shell_context_t *context, char *line);

// Path Utility
int get_cmd_path(ms_shell_context_t *context, char *search, char *full_path);

// Explode utility
char **my_explode(char const *str, char const *separator);
void free_str_arr(char **str_arr);
char *my_join(char const *separator, char **str_arr);
int count_words(char const *str, char const *separator);

// Miscellaneous Utils
int my_isnumerical(char c);
int my_str_isnumerical(char const *str);
int my_strchr(char const *str, char c);
int error(char const *format, ...);
int my_getexit(char const *str);

// Keymap Utility
bool km_has(char const *key, keymap_t *keymap);
char *km_get(char const *key, keymap_t *keymap);
void km_set(char const *key, char const *value, keymap_t **keymap);
void km_unset(char const *key, keymap_t **keymap);
char *km_get_or_default(char const *key, keymap_t *keymap,
    char *def);

// Env utils
void ms_populate_env_from_dump(char **env_dump, ms_shell_context_t *context);
char **ms_dump_env(ms_shell_context_t *context);

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

// History misc
char *expand_history(char *line, ms_shell_context_t *context);
void fill_the_history(ms_shell_context_t *context, char *line);
int check_display(char *line, ms_shell_context_t *ctx);
char *number_case(char *line, ms_shell_context_t *ctx);
char *double_bang(char *line, ms_shell_context_t *ctx);

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
