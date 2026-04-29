/*
** EPITECH PROJECT, 2026
** minishell1
** File description:
** MiniShell utility managing
** the ENV (setting, getting...)
** its values from the linked
** list in the shell context
** Author:
** Amélie Ambleton--Guth
** February 2026
*/

#include <stdlib.h>

#include "minishell1.h"

static char *km_get_internal(char const *key, keymap_t *keymap,
    bool allow_empty)
{
    ms_env_entry_t *entry;

    for (list_t *env = keymap; env; env = env->next) {
        entry = env->data;
        if (!my_strcmp(entry->key, key))
            return entry->value;
    }
    if (allow_empty)
        return "";
    return NULL;
}

bool km_has(char const *key, keymap_t *keymap)
{
    return !!km_get_internal(key, keymap, false);
}

char *km_get(char const *key, keymap_t *keymap)
{
    return km_get_internal(key, keymap, true);
}

char *km_get_or_default(char const *key, keymap_t *keymap,
    char *def)
{
    char *value = km_get_internal(key, keymap, false);

    if (value)
        return value;
    return def;
}

void km_set(char const *key, char const *value, keymap_t **keymap)
{
    ms_env_entry_t *entry;

    for (; keymap && *keymap; keymap = &(*keymap)->next) {
        entry = (*keymap)->data;
        if (!my_strcmp(entry->key, key)) {
            safe_free(&entry->value);
            entry->value = my_strdup(value);
            return;
        }
    }
    entry = my_calloc(1, sizeof(ms_env_entry_t));
    entry->key = my_strdup(key);
    entry->value = my_strdup(value);
    ll_unshift(keymap, entry);
}

void km_unset(char const *key, keymap_t **keymap)
{
    ms_env_entry_t *entry;

    for (; keymap && *keymap; keymap = &(*keymap)->next) {
        entry = (*keymap)->data;
        if (!my_strcmp(entry->key, key)) {
            free(entry->value);
            free(entry->key);
            free(entry);
            ll_shift(keymap);
            return;
        }
    }
}
