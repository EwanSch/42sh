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

void msle_move_left(ms_line_editor_t *lined, int mod, int payload)
{
    if (lined->caret_pos > 0)
        lined->caret_pos--;
}

void msle_move_right(ms_line_editor_t *lined, int mod, int payload)
{
    if (lined->caret_pos < lined->text_len)
        lined->caret_pos++;
}

void msle_move_start(ms_line_editor_t *lined, int mod, int payload)
{
    lined->caret_pos = 0;
}

void msle_move_end(ms_line_editor_t *lined, int mod, int payload)
{
    lined->caret_pos = lined->text_len;
}
