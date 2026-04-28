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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <threads.h>
#include <unistd.h>
#include <linux/limits.h>
#include <sys/wait.h>
#include "minishell1.h"
#include "minishell2.h"
#include "ms_builtins.h"

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

int process_line(ms_shell_context_t *context, char *line)
{
    list_t *tokens;
    char *expanded;

    if (!context || !line)
        return 1;
    expanded = expand_paths(line, context);
    if (!expanded)
        return 1;
    tokens = cut_words(expanded, context);
    free(expanded);
    if (!tokens)
        return 1;
    return ms_runner(tokens, context);
}

static void env_to_var(char const *env_name, char const *var_name,
    char *default_value, ms_shell_context_t *ctx)
{
    char const *value = km_get_or_default(env_name, ctx->env, default_value);

    km_set(var_name, value, &ctx->variables);
}

static void prepare_variables(ms_shell_context_t *context)
{
    char *cwd = getcwd(NULL, 0);

    km_set(MS_PROMPT_DEFAULT, DEFAULT_NORMAL_PROMPT, &context->variables);
    km_set(MS_PROMPT_FOLLOWUP, DEFAULT_FOLLOWUP_PROMPT, &context->variables);
    km_set(MS_VAR_CWD, cwd, &context->variables);
    env_to_var(MS_ENV_HOME, MS_VAR_HOME, NULL, context);
    env_to_var(MS_ENV_PATH, MS_VAR_PATH, "/usr/bin:/bin", context);
    free(cwd);
}

static int msle_mainloop(ms_shell_context_t *context, ms_line_editor_t *lined)
{
    char c;

    while (1) {
        display_prompt(context, lined);
        if (read(STDIN_FILENO, &c, 1) != 1)
            return 84;
        if (c == 0x04)
            break;
        if (msle_special_key(context, lined, c))
            continue;
        msle_add_character(lined, c);
    }
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
        context->last_exit_status = process_line(context, context->line_buffer);
        free(context->line_buffer);
    }
    return -1;
}

int main(int argc, char **argv, char **env)
{
    struct termios orig_termios;
    ms_shell_context_t context = {0};
    ms_line_editor_t lined = {0};
    int return_value = 0;

    enable_raw_mode(&orig_termios);
    context.is_interactive = isatty(STDIN_FILENO);
    ms_populate_env_from_dump(env, &context);
    prepare_variables(&context);
    context.reader = lr_from_stream(stdin);
    return_value = context.reader ? mainloop(&context, &lined) : 84;
    safe_free(&lined.history[lined.history_index]);
    disable_raw_mode(&orig_termios);
    ms_teardown(&context);
    if (context.is_interactive)
        my_putstr("\n");
    return return_value == -1 ? context.last_exit_status : return_value;
}
