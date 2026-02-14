/*
** EPITECH PROJECT, 2026
** minishell1
** File description:
** Project's entry point
** Auto-Generated with Episetup by Amélie
** Author:
** Amélie Ambleton--Guth
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

void ms_teardown(ms_shell_context_t *context)
{
    list_t *env = context->env;
    ms_env_entry_t *entry;

    while (env) {
        entry = ll_shift(&env);
        free(entry->key);
        free(entry->value);
        free(entry);
    }
    if (context->last_working_dir)
        free(context->last_working_dir);
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

static void display_prompt(ms_shell_context_t *context)
{
    char *cwd = getcwd(NULL, 0);

    my_printf("[%d %s]$ ", context->last_exit_status, cwd);
    free(cwd);
}

static size_t expand_tilde_size(ms_shell_context_t *context, char *line)
{
    size_t expansion_size = 0;
    char *expansion_string = ms_get_env_value("HOME", context);

    for (int chr = 0; line[chr]; chr++) {
        if (line[chr] == '~' && (chr == 0 || line[chr - 1] == ' '))
            expansion_size += my_strlen(expansion_string);
        else
            expansion_size += 1;
    }
    return expansion_size;
}

static char *expand_tilde(ms_shell_context_t *context, char *line)
{
    int expansion_size = 0;
    char *expansion_string = ms_get_env_value("HOME", context);
    char *expanded_line = my_calloc(expand_tilde_size(context, line) + 1,
        sizeof(char));

    if (!expanded_line)
        return NULL;
    for (int chr = 0; line[chr]; chr++) {
        if (line[chr] == '~' && (chr == 0 || line[chr - 1] == ' ')) {
            my_strcpy(expanded_line + expansion_size, expansion_string);
            expansion_size += my_strlen(expansion_string);
        } else {
            expanded_line[expansion_size] = line[chr];
            expansion_size += 1;
        }
    }
    expanded_line[expansion_size] = '\0';
    return expanded_line;
}

static void process_line(ms_shell_context_t *context, char *line)
{
    char *expanded_line = expand_tilde(context, line);
    char **args;

    if (expanded_line) {
        args = my_explode(expanded_line, " \t\n");
        free(expanded_line);
    } else
        args = my_explode(line, " \t\n");
    context->last_exit_status = run_command(args, context);
    free_str_arr(args);
}

int main(int argc, char **argv, char **env)
{
    struct termios orig_termios;
    enable_raw_mode(&orig_termios);
    char c;

    size_t bufsize = 0;
    ssize_t text_len = 0;
    ssize_t caret_pos = 0;
    char *my_reallocated_line = NULL;
    char *prompt = "[minishell]$ ";
    char *history[50] = {0};
    int history_index = 0;
    int history_top = 0;
    char *line_save = NULL;

    while (1) {
        printf("\r%s%s\x1b[K", prompt, history[history_index] ?
            history[history_index] : "");
        fflush(stdout);
        if (text_len > caret_pos) {
            char seq[32];
            sprintf(seq, "\x1b[%zdD", text_len - caret_pos);
            write(1, seq, strlen(seq));
        }

        if (read(STDIN_FILENO, &c, 1) != 1)
            break;
        if (c == '\x1b') {
            if (read(STDIN_FILENO, &c, 1) != 1 || c != '[') {
                printf("bha évite\n");
                continue;
            }
            // We are getting an escape sequence...
            char seq[64];
            int i = 0;

            while (i < sizeof(seq)-1) {
                read(STDIN_FILENO, &seq[i], 1);
                if (seq[i] >= '@' && seq[i] <= '~')
                    break;
                i++;
            }
            seq[i+1] = '\0';
            //printf("\x1b[%s", seq);
            //fflush(stdout);
            //printf("Aoe ça échappe %s oklm\n", seq);
            char *saveptr;
            strtok_r(seq, ";", &saveptr);
            if (saveptr && *saveptr) {
                int mod = atoi(saveptr);
                printf("Modif %d, ", mod);
            }
            if (seq[i] == '~') {
                int charcode = atoi(seq);
                printf("codepoint %d\n", charcode);
            } else if (seq[i] == 'A') {
                // printf("up\n");
                if (history_index > 0) {
                    if (line_save) {
                        my_strcpy(history[history_index], line_save);
                        free(line_save);
                        line_save = NULL;
                    }
                    history_index--;
                    if (history[history_index]) {
                        line_save = my_strdup(history[history_index]);
                    }
                    //my_line = history[history_index];
                    if (history[history_index])
                        text_len = my_strlen(history[history_index]);
                    else
                        text_len = 0;
                    caret_pos = text_len;
                }
            } else if (seq[i] == 'B') {
                // printf("down\n");
                if (history_index < history_top) {
                    if (line_save) {
                        my_strcpy(history[history_index], line_save);
                        free(line_save);
                        line_save = NULL;
                    }
                    history_index++;
                    if (history[history_index]) {
                        line_save = my_strdup(history[history_index]);
                    }
                    //my_line = history[history_index];
                    if (history[history_index])
                        text_len = my_strlen(history[history_index]);
                    else
                        text_len = 0;
                    caret_pos = text_len;
                    if (history_index == history_top) {
                        if (line_save) {
                            free(line_save);
                            line_save = NULL;
                        }
                    }
                }
            } else if (seq[i] == 'C') {
                //printf("right\n");
                if (caret_pos < text_len)
                    caret_pos++;
            } else if (seq[i] == 'D') {
                //printf("left\n");
                if (caret_pos > 0)
                    caret_pos--;
            } else if (seq[i] == 'F') {
                //printf("left\n");
                caret_pos = text_len;
            } else if (seq[i] == 'H') {
                //printf("left\n");
                caret_pos = 0;
            } else {
                printf("unsupported kind %c\n", seq[i]);
            }

            continue;
        }
        // printf("euh 0b%08hhb / 0x%02hhX / %hhu (%c)\n", c, c, c, c);
        if (text_len >= bufsize) {
            my_reallocated_line = realloc(history[history_index], bufsize + 256);
            if (!my_reallocated_line) {
                perror("realloc");
                exit(EXIT_FAILURE);
            }
            history[history_index] = my_reallocated_line;
        }
        if (c == 0x7F) {
            if (text_len > 0 && caret_pos == text_len) {
                caret_pos--;
                text_len--;
                history[history_index][caret_pos] = '\0';
            } else if (text_len > 0 && caret_pos > 0) {
                for (int i = caret_pos - 1; i < text_len; i++) {
                    history[history_index][i] = history[history_index][i + 1];
                }
                caret_pos--;
                text_len--;
            }
            continue;
        }
        if (c == '\n') {
            history[history_index][text_len] = '\0';
            printf("\nD'accodac frère\n");
            if (history_top >= 49) {
                free(history[0]);
                for (int i = 0; i <= 49; i++) {
                    history[i] = history[i + 1];
                }
                history_top--;
            }
            text_len = 0;
            caret_pos = 0;

            if (history[history_top])
                free(history[history_top]);
            history[history_top] = my_strdup(history[history_index]);
            if (line_save) {
                my_strcpy(history[history_index], line_save);
                free(line_save);
                line_save = NULL;
            }
            history_top++;
            history_index = history_top;
            history[history_index] = NULL;
            bufsize = 0;
            continue;
            //process_line(NULL, my_line);
        }
        if (c == '\4') {
            break; // EOF
        }

        // We want to append the new character at the position of the carret
        // Check if we are at the end
        if (caret_pos == text_len) {
            history[history_index][caret_pos++] = c;
            history[history_index][caret_pos] = '\0';
            text_len++;
        } else {
            // We need to move the array...
            for (int i = text_len; i > caret_pos; i--) {
                history[history_index][i] = history[history_index][i - 1];
            }
            history[history_index][caret_pos++] = c;
            //my_line[caret_pos] = '\0';
            text_len++;
        }
    }
    if (history[history_index])
        free(history[history_index]);
    disable_raw_mode(&orig_termios);

    /*ms_shell_context_t context = {0};
    size_t bufsize = 0;
    char *buf = NULL;
    ssize_t read;

    ms_populate_env_from_dump(env, &context);
    while (1) {
        display_prompt(&context);
        read = getline(&buf, &bufsize, stdin);
        if (read == -1)
            break;
        if (read == 0)
            continue;
        buf[read - 1] = '\0';
        process_line(&context, buf);
    }
    free(buf);
    ms_teardown(&context);
    my_putstr("\n");
    return context.last_exit_status;*/
}
