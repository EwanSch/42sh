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
#include "minishell2.h"

void msle_refresh_cursor_position(ms_line_editor_t *lined)
{
    char seq[32];

    if (lined->text_len > lined->caret_pos) {
        sprintf(seq, "\x1b[%zdD", lined->text_len - lined->caret_pos);
        write(1, seq, my_strlen(seq));
    }
}

void display_prompt(ms_shell_context_t *context, ms_line_editor_t *lineed)
{
    ms_prompt(context, MS_PROMPT_DEFAULT);
    msle_refresh_cursor_position(lineed);
}

int msle_extend_input_buffer(ms_line_editor_t *lined)
{
    char *new_input_buffer;

    lined->bufsize += MYSH_IN_BUF_EXTENSION_STEP;
    new_input_buffer = realloc(lined->input_buffer, lined->bufsize);
    if (!new_input_buffer) {
        perror("realloc");
        return 1;
    }
    lined->input_buffer = new_input_buffer;
    return 0;
}

void msle_add_character(ms_line_editor_t *lined, char const c)
{
    if (lined->caret_pos >= lined->bufsize) {
        if (msle_extend_input_buffer(lined))
            return;
    }
    if (lined->caret_pos == lined->text_len) {
        lined->input_buffer[lined->caret_pos] = c;
        lined->input_buffer[lined->caret_pos + 1] = '\0';
    } else {
        for (int i = (int) lined->text_len; i > lined->caret_pos; i--)
            lined->input_buffer[i] = lined->input_buffer[i - 1];
        lined->input_buffer[lined->caret_pos] = c;
    }
    lined->text_len++;
    lined->caret_pos++;
}

int msle_special_key(ms_shell_context_t *ctx, ms_line_editor_t *lined, char c)
{
    switch (c) {
        case 0x7F:
        case 0x08:
            msle_hit_backspace(lined);
            return 1;
        case 0x1B:
            msle_escape_sequence(lined);
            return 1;
        case '\n':
            msle_hit_enter(ctx, lined);
            return 1;
        case '\t':
            msle_hit_tab(lined);
            return 1;
        default:
            return 0;
    }
}
