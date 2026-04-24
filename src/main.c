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
    if (!my_strcmp(args[0], "exit"))
        return run_exit(args, context);
    if (!my_strcmp(args[0], "cd"))
        return run_cd(args, context);
    if (!my_strcmp(args[0], "setenv"))
        return ms_env_setenv(args + 1, context);
    if (!my_strcmp(args[0], "unsetenv"))
        return ms_env_unset(args + 1, context);
    if (!my_strcmp(args[0], "env"))
        return ms_env_show(args + 1, context);
    return run_other(args, context);
}

int process_line(ms_shell_context_t *context, char *line)
{
    list_t *tokens;
    char *expanded;

    line = expand_history(line, context);
    if (!context || !line)
        return 1;
    expanded = expand_paths(line, context);
    if (!expanded)
        return 1;
    tokens = cut_words(expanded);
    free(expanded);
    if (!tokens)
        return 1;
    return ms_runner(tokens, context);
}

static void prepare_variables(ms_shell_context_t *context)
{
    km_set(MS_PROMPT_DEFAULT, DEFAULT_NORMAL_PROMPT, &context->variables);
    km_set(MS_PROMPT_FOLLOWUP, DEFAULT_FOLLOWUP_PROMPT, &context->variables);
}
/*
static int main_loop(ms_shell_context_t *context, linereader_t *lr)
{
    if (!lr || !context)
        return -1;
    ms_prompt(context, MS_PROMPT_DEFAULT);
    context->line_buffer = lr_read(lr);
    if (!context->line_buffer)
        return -1;
    context->last_exit_status = process_line_v2(context, context->line_buffer);
    free(context->line_buffer);
    return 0;
}*/

static int msle_mainloop(ms_shell_context_t *context, ms_line_editor_t *lined)
{
    char c;
    int res = 0;

    while (1) {
        display_prompt(context, lined);
        if (read(STDIN_FILENO, &c, 1) != 1)
            return 84;
        if (c == 0x04)
            break;
        if (msle_special_key(context, lined, c))
            continue;
        if ((lined->text_len + 1) >= lined->bufsize &&
            msle_extend_input_buffer(lined))
            continue;
        msle_add_character(lined, c);
    }
    return res;
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
    if (!context.reader)
        return_value = 84;
    if (return_value == 0)
        return_value = msle_mainloop(&context, &lined);
    safe_free(&lined.history[lined.history_index]);
    disable_raw_mode(&orig_termios);
    ms_teardown(&context);
    if (context.is_interactive)
        my_putstr("\n");
    return return_value == -1 ? context.last_exit_status : return_value;
}
