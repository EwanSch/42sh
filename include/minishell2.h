/*
** EPITECH PROJECT, 2025
** Linked List Libs
** File description:
** Include file for linked list functions
** Author:
** Amélie Ambleton--Guth
*/

#ifndef MINISHELL2_H
    #define MINISHELL2_H

    #include <stdbool.h>
    #include "benjalib.h"
    #include "minishell1.h"

    #define IS_SEPARATOR(x) (x == ' ' || x == '\t' || x == '\n')

    #define PARSER_QUOTING(x) ((x)->quote_mode != MS_QUOTE_NONE)
    #define PARSER_ESCAPING(x) ((x)->backslashed || PARSER_QUOTING(x))

    #define VIS_VALID(x, y, z) (!(x) || y->type != z)

    #define RECORD_SEPARATOR '\x1e'

    #define RD_OUT(x) (x == MS_TOKEN_GREATER || x == MS_TOKEN_DOUBLE_GREATER)
    #define RD_IN(x) (x == MS_TOKEN_LESS || x == MS_TOKEN_DOUBLE_LESS)

    #define MS_PROMPT_DEFAULT "prompt"
    #define MS_PROMPT_FOLLOWUP "prompt2"

    #define DEFAULT_NORMAL_PROMPT "[%? %/]$ "
    #define DEFAULT_FOLLOWUP_PROMPT "? "

    #define MS_VAR_HOME "home"
    #define MS_VAR_CWD "cwd"
    #define MS_VAR_PATH "path"

    #define MS_ENV_HOME "HOME"
    #define MS_ENV_PATH "PATH"

    #define MS_NO_HISTORY "%d: Event not found.\n"
    #define MS_NO_HISTORY2 "%s: Event not found.\n"
    #define MAX_CMD 4096
    #define HISTORY_CMD "!!"

typedef struct ms_parser_s ms_parser_t;
typedef struct ms_grammar_parser_s ms_grammar_parser_t;
typedef struct ms_syntax_tree_s ms_syntax_tree_t;

typedef enum {
    MS_QUOTE_NONE,
    MS_QUOTE_SINGLE,
    MS_QUOTE_DOUBLE,
    MS_QUOTE_BACKTICK,
    MS_QUOTE_CSTYLE,
} ms_quote_mode_t;

typedef enum {
    MS_TOKEN_WORD,
    MS_TOKEN_PIPE,
    MS_TOKEN_SEMICOLON,
    MS_TOKEN_AMPERSAND,
    MS_TOKEN_LESS,
    MS_TOKEN_GREATER,
    MS_TOKEN_LEFT_BRACKET,
    MS_TOKEN_RIGHT_BRACKET,
    MS_TOKEN_DOUBLE_LESS,
    MS_TOKEN_DOUBLE_GREATER,
    MS_TOKEN_OR,
    MS_TOKEN_AND,
    MS_TOKEN_EOF
} ms_token_type_t;

typedef enum {
    MS_TREE_ROOT,
    MS_TREE_WORD,
    MS_TREE_PIPELINE,
    MS_TREE_SEQUENCE,
    MS_TREE_AND_OR,
    MS_TREE_REDIRECTION,
    MS_TREE_COMMAND,
    MS_TREE_SIMPLE_COMMAND
} ms_tree_type_t;

typedef enum {
    MSE_AMBIGUOUS_OUTPUT,
    MSE_AMBIGUOUS_INPUT,
    MSE_MISSING_NAME_REDIRECT,
    MSE_NULL_COMMAND,
    MSE_COMMAND_NOT_FOUND,
} ms_error_t;

typedef struct {
    ms_token_type_t type;
    char *word_value;
    int line;
} ms_token_t;

typedef struct {
    size_t size;
    char *buffer;
} ms_stringbuffer_t;

struct ms_parser_s {
    bool backslashed;
    int curr_char;
    char *str_buf;
    ms_quote_mode_t quote_mode;
    list_t *tokens;
};

struct ms_grammar_parser_s {
    list_t *tokens;
    bool errored;
    ms_shell_context_t *ctx_ref;
};

struct ms_syntax_tree_s {
    list_t *children;
    ms_tree_type_t type;
    ms_shell_context_t *ctx_ref;
    ms_syntax_tree_t *root_ref;
};

// Error Utils
int ms_fail(ms_shell_context_t *context, ms_error_t error);
int ms_fail_parse(ms_shell_context_t *context, ms_error_t error,
    ms_token_t *token);
int ms_error(char *error);

// LineReader util
linereader_t *lr_new(const char *filename);
linereader_t *lr_from_stream(FILE *stream);
void lr_close(linereader_t *fr);
char *lr_read(linereader_t *fr);

// Misc Utils
void free_batch(int count, ...);
void free_grammar(ms_grammar_parser_t *grammar);
void free_ast(ms_syntax_tree_t *node);
void free_token(ms_token_t *token);

// Enum to string utils
char *ms_tree_type_to_str(ms_tree_type_t type);
char *ms_token_to_str(ms_token_t *token);
void print_tree(ms_syntax_tree_t *tree, int depth);

// Grammar Parser Utils
ms_token_t *gr_match(ms_grammar_parser_t *grammar, ms_token_type_t type,
    bool destroy);
bool gr_testahead(ms_grammar_parser_t *grammar, ms_token_type_t type);
bool gr_testfor(ms_grammar_parser_t *grammar, ms_token_type_t type);
ms_token_t *gr_consume(ms_grammar_parser_t *grammar);
bool gr_at_end(ms_grammar_parser_t *grammar);

// Grammar Parser
ms_syntax_tree_t *ms_generate_ast(list_t *tokens, ms_shell_context_t *context);
bool verify_pipeline(ms_syntax_tree_t *pipeline);


// Running
int ms_runner(list_t *tokens, ms_shell_context_t *context);
int run_command(char **args, ms_shell_context_t *context);
int visit_simple_command(ms_syntax_tree_t *node, ms_shell_context_t *context,
    int fdin, int fdout);
int ms_prompt(ms_shell_context_t *context, char *type);

// Pipelines
int pipeline_handler(ms_syntax_tree_t *node, ms_shell_context_t *context);

// Lexing
list_t *cut_words(char *string, ms_shell_context_t *context);

// Path expansion
char *expand_paths(char *string, ms_shell_context_t *ctx);

// Redirections
int visit_redirection(ms_syntax_tree_t *node,
    ms_shell_context_t *context, int *fdin, int *fdout);

int my_strindexof(char const *str, char c);

char *str_destroyer(char *str, int *charac);

int mismatch(char *str);

bool is_alias(ms_grammar_parser_t *grammar);
char *ll_to_str(list_t **ll, char *in_btw);


#endif
