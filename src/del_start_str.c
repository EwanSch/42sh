/*
** EPITECH PROJECT, 2025
** aaa
** File description:
** aab
*/

#include "../include/my.h"
#include <stdlib.h>

char *del_start_str(char *str, int amount)
{
    int str_len = 0;
    int futur_size = 0;

    if (!str || str[0] == 0)
        return NULL;
    str_len = my_strlen(str);
    if (amount > str_len || amount < 0)
        return NULL;
    futur_size = str_len - amount;
    for (int i = 0; i < futur_size; i++)
        str[i] = str[i + amount];
    str[futur_size] = 0;
    return str;
}
