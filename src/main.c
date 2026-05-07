/*
** EPITECH PROJECT, 2026
** minishell1
** File description:
** Project's entry point
** Auto-Generated with Episetup by Amélie
** Author:
** Amélie Ambleton--Guth
** Feb 2026
*/

#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <threads.h>
#include <unistd.h>
#include <linux/limits.h>
#include <sys/wait.h>

#include "minishell1.h"
#include "minishell2.h"
#include "globbing.h"
#include "shell.h"
#include "ms_builtins.h"
#include "ms_grammar.h"
#include "line_editor.h"

void free_alias(alias_t *alias)
{
    alias_t *temp = NULL;
    while (alias) {
        temp = alias->next;
        free(alias->alias);
        free(alias->name);
        free(alias);
        alias = temp;
    }
}

void ms_teardown(ms_shell_context_t *context)
{
    ms_env_entry_t *entry;

    while (context->env) {
        entry = ll_shift(&context->env);
        safe_free(&entry->key);
        safe_free(&entry->value);
        safe_free(&entry);
    }
    while (context->variables) {
        entry = ll_shift(&context->variables);
        safe_free(&entry->key);
        safe_free(&entry->value);
        safe_free(&entry);
    }
    free_alias(context->alias);
    safe_free(&context->last_working_dir);
    safe_free(&context->line_buffer);
    if (context->reader)
        lr_close(context->reader);
    context->reader = NULL;
}

int run_command(char **args, ms_shell_context_t *context)
{
    if (!args || !args[0])
        return 0;
    for (int i = 0; ms_builtins_list[i].name; i++) {
        if (my_strcmp(ms_builtins_list[i].name, args[0]) == 0) {
            return ms_builtins_list[i].callback(context, args + 1);
        }
    }
    return run_other(args, context);
}

int process_line(ms_shell_context_t *context, char **line)
{
    list_t *tokens;
    char *expanded = NULL;

    *line = expand_history(*line, context);
    if (!context || !*line)
        return 1;
    fill_the_history(context, *line);
    if (check_parentheses_basic(*line))
        return 1;
    expanded = expand_paths(*line, context);
    if (!expanded)
        return 1;
    tokens = cut_words(expanded, context);
    free(expanded);
    if (!tokens || apply_globbing(&tokens, context))
        return 1;
    return ms_runner(tokens, context);
}

static void env_to_var(char *env_name, char *var_name,
    char *default_value, ms_shell_context_t *ctx)
{
    char *value = km_get_or_default(env_name, ctx->env, default_value);

    km_set(var_name, value, &ctx->variables);
}

static void div_prepare_variables(ms_shell_context_t *context, char **argv,
    int argc)
{
    set_term_variable(context);
    set_cwd_variable(context);
    save_argv(context, argv, argc);
}

static void prepare_variables(ms_shell_context_t *context, char **argv,
    int argc)
{
    km_set(MS_PROMPT_DEFAULT, DEFAULT_NORMAL_PROMPT, &context->variables);
    km_set(MS_PROMPT_FOLLOWUP, DEFAULT_FOLLOWUP_PROMPT, &context->variables);
    km_set(MS_VAR_ADDSUFFIX, NULL, &context->variables);
    km_set(MS_VAR_ARGV, NULL, &context->variables);
    km_set(MS_VAR_AUTOLOGOUT, NULL, &context->variables);
    km_set(MS_VAR_HOME, km_get_or_default(MS_ENV_HOME, context->env, NULL),
        &context->variables);
    km_set(MS_VAR_PATH, km_get_or_default(MS_ENV_PATH, context->env, NULL),
        &context->variables);
    km_set(MS_VAR_GROUP, km_get_or_default(MS_ENV_GROUP, context->env, NULL),
        &context->variables);
    km_set(MS_VAR_SHLVL, km_get_or_default(MS_ENV_SHLVL, context->env, NULL),
        &context->variables);
    km_set(MS_VAR_USER, km_get_or_default(MS_ENV_USER, context->env, NULL),
        &context->variables);
    km_set(MS_VAR_SHELL, "/bin/tcsh", &context->variables);
    km_set(MS_VAR_STATUS, "0", &context->variables);
    div_prepare_variables(context, argv, argc);
}

static int msle_mainloop(ms_shell_context_t *context, ms_line_editor_t *lined)
{
    char c;

    while (1) {
        display_prompt(context, lined);
        if (read(STDIN_FILENO, &c, 1) != 1) {
            safe_free(&lined->history[lined->history_index]);
            return 84;
        }
        if (command_eof(c, context))
            break;
        if (c == 0x04)
            continue;
        if (msle_special_key(context, lined, c))
            continue;
        msle_add_character(lined, c);
    }
    ms_free_history(context);
    safe_free(&lined->history[lined->history_index]);
    return -1;
}

static int mainloop(ms_shell_context_t *context, ms_line_editor_t *lined)
{
    if (context->is_interactive)
        return msle_mainloop(context, lined);
    while (1) {
        context->line_buffer = lr_read(context->reader);
        if (!context->line_buffer)
            break;
        context->last_exit_status =
            process_line(context, &context->line_buffer);
        free(context->line_buffer);
    }
    return -1;
}

int main(int argc, char **argv, char **env)
{
    struct termios orig_termios = {0};
    ms_shell_context_t context = {0};
    ms_line_editor_t lined = {0};
    int return_value = 0;

    enable_raw_mode(&orig_termios);
    context.is_interactive = isatty(STDIN_FILENO);
    ms_populate_env_from_dump(env, &context);
    prepare_variables(&context, argv, argc);
    context.reader = lr_from_stream(stdin);
    return_value = context.reader ? mainloop(&context, &lined) : 84;
    disable_raw_mode(&orig_termios);
    ms_teardown(&context);
    msle_history_clear(&lined, 0, 0);
    if (context.is_interactive)
        my_putstr("\n");
    return return_value == -1 ? context.last_exit_status : return_value;
}
