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

void msle_hit_enter(ms_shell_context_t *context, ms_line_editor_t *lined)
{
    if (lined->input_buffer)
        lined->input_buffer[lined->text_len] = '\0';
    safe_free(&lined->history[0]);
    lined->history[0] = my_strdup(lined->input_buffer);
    safe_free(&lined->history[MYSH_HISTORY_SIZE - 1]);
    if (lined->history_top < MYSH_HISTORY_SIZE - 1)
        lined->history_top++;
    for (int i = lined->history_top; i > 0; i--)
        lined->history[i] = lined->history[i - 1];
    lined->history[0] = NULL;
    lined->text_len = 0;
    lined->caret_pos = 0;
    safe_free(&lined->input_buffer);
    lined->bufsize = 0;
    lined->history_index = 0;
    my_fputs(1, "\n");
    fill_the_history(context, lined);
    process_line(context, lined->history[1]);
}

void msle_hit_backspace(ms_line_editor_t *lined)
{
    if (lined->text_len <= 0 || lined->caret_pos <= 0)
        return;
    if (lined->caret_pos == lined->text_len)
        lined->input_buffer[lined->caret_pos - 1] = '\0';
    else
        for (int i = (int) lined->caret_pos - 1; i < lined->text_len; i++)
            lined->input_buffer[i] = lined->input_buffer[i + 1];
    lined->caret_pos--;
    lined->text_len--;
}

void msle_hit_tab(ms_line_editor_t *lined)
{
    write(1, "\x07", 1);
}
