/*
** EPITECH PROJECT, 2026
** minishell1
** File description:
** MiniShell translation unit
** containing functions related
** to reading raw terminal inputs
** allowing escape sequences
** processing
** Author:
** Amélie Ambleton--Guth
** February 2026
*/

#include <termios.h>
#include <unistd.h>

void enable_raw_mode(struct termios *orig_termios)
{
    struct termios raw = *orig_termios;

    tcgetattr(STDIN_FILENO, orig_termios);
    tcgetattr(STDIN_FILENO, &raw);
    raw.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void disable_raw_mode(struct termios *orig_termios)
{
    orig_termios->c_lflag |= (ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, orig_termios);
}
