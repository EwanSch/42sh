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

void msle_history_up(ms_line_editor_t *lined, int mod, int payload)
{
    if (lined->history_index >= lined->history_top)
        return;
    if (lined->history_index == 0)
        lined->history[0] = my_strdup(lined->input_buffer);
    lined->history_index++;
    if (lined->history[lined->history_index]) {
        safe_free(&lined->input_buffer);
        lined->input_buffer = my_strdup(lined->history[lined->history_index]);
    }
    lined->text_len = lined->input_buffer ? my_strlen(lined->input_buffer) : 0;
    lined->caret_pos = lined->text_len;
    lined->bufsize = lined->text_len + 1;
}

void msle_history_down(ms_line_editor_t *lined, int mod, int payload)
{
    if (lined->history_index <= 0)
        return;
    lined->history_index--;
    safe_free(&lined->input_buffer);
    lined->input_buffer = my_strdup(lined->history[lined->history_index]);
    lined->text_len = lined->input_buffer ? my_strlen(lined->input_buffer) : 0;
    lined->bufsize = lined->text_len + 1;
    lined->caret_pos = lined->text_len;
}
