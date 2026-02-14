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

#include "minishell1.h"

#include <string.h>

static ms_editor_command_t const msle_commands[] = {
    {'A', .callback = &msle_history_up},
    {'B', .callback = &msle_history_down},
    {'C', .callback = &msle_move_right},
    {'D', .callback = &msle_move_left},
    {'H', .callback = &msle_move_start},
    {'F', .callback = &msle_move_end},
    {'~', .callback = &msle_misc_codepoint},
    {0, NULL}
};

void msle_read_full_escape(char *sequence, int seq_size, int *i)
{
    while (*i < seq_size - 1) {
        if (read(STDIN_FILENO, &sequence[*i], 1) != 1)
            break;
        if (sequence[*i] >= '@' && sequence[*i] <= '~')
            break;
        (*i)++;
    }
    sequence[*i + 1] = '\0';
}

int msle_get_modifier_key(char *sequence)
{
    char *saveptr;

    strtok_r(sequence, ";", &saveptr);
    if (saveptr && *saveptr)
        return atoi(saveptr);
    return 0;
}

void msle_escape_seq_action(ms_line_editor_t *lined, char *seq, int mod)
{
    int const length = my_strlen(seq);
    int const end = length - 1;
    int const payload = atoi(seq);

    for (ms_editor_command_t const *cmd = msle_commands; cmd->key; cmd++) {
        if (cmd->key == seq[end]) {
            cmd->callback(lined, mod, payload);
            return;
        }
    }
}

void msle_escape_sequence(ms_line_editor_t *lined)
{
    char c = 0;
    int mod = 0;
    int seq_len = 0;
    char seq[MYSH_ESC_SEQ_MAXLEN] = {0};

    if (read(STDIN_FILENO, &c, 1) != 1 || c != '[')
        return;
    msle_read_full_escape(seq, MYSH_ESC_SEQ_MAXLEN, &seq_len);
    mod = msle_get_modifier_key(seq);
    msle_escape_seq_action(lined, seq, mod);
}
