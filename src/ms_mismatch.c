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

#include <stdio.h>

#include "minishell1.h"
#include "ms_grammar.h"

static int find_match(char *str, int *stay, int i, char search)
{
    if (*stay || str[i] == search) {
        *stay = 1;
        if (str[i + 1] == search) {
            *stay = 0;
            return 1;
        }
    }
    return 0;
}

int check_for_match(char *str, char search)
{
    int stay = 0;

    for (int i = 0; str[i]; i++) {
        if (find_match(str, &stay, i, search))
            break;
    }
    return stay ? 1 : 0;
}

static void find_brace(char *str, int *stay, int *buf, int i)
{
    if (*stay || str[i] == '{') {
        if (*stay == 0 && str[i + 1] == ' ') {
            *buf = 1;
        }
        *stay = 1;
        if (str[i + 1] == '}') {
            *stay = 0;
            return;
        }
    }
}

static int check_brace(char *str)
{
    int stay = 0;
    int buf = 0;

    for (int i = 0; str[i]; i++) {
        find_brace(str, &stay, &buf, i);
    }
    if (buf)
        return 0;
    return stay == 1 ? 1 : 0;
}

int all_quote(char *str, int i)
{
    if (str[i] == '\"' || str[i] == '\'' || str[i] == '`') {
        if (check_for_match(str, str[i])) {
            printf("Unmatched '%c'.\n", str[i]);
            return 1;
        }
    }
    return 0;
}

int mismatch(char *str)
{
    for (int i = 0; str[i]; i++)
        if (all_quote(str, i))
            return 1;
    if (check_brace(str)) {
        printf("Missing '}'.\n");
        return 1;
    }
    return 0;
}

int check_parentheses_basic(const char *line)
{
    int parentheses = 0;

    for (int i = 0; line[i]; i++) {
        if (line[i] == '(')
            parentheses++;
        if (line[i] == ')')
            parentheses--;
        if (parentheses < 0) {
            write(2, MS_RIGHT_BRACKET_MUCH,
                my_strlen(MS_RIGHT_BRACKET_MUCH));
            return 1;
        }
    }
    if (parentheses > 0) {
        write(2, MS_LEFT_BRACKET_MUCH,
            my_strlen(MS_LEFT_BRACKET_MUCH));
        return 1;
    }
    return 0;
}
