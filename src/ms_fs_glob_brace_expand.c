/*
** EPITECH PROJECT, 2026
** G-PSU-200-STG-2-1-42sh-1
** File description:
** ms_fs_glob_brace_expand
*/

#include "globbing.h"

static char *build_pattern(const char *pattern,
    char *open, char *close, char *part)
{
    char *res = my_calloc(1024, 1);

    if (!res)
        return NULL;
    strncat(res, pattern, open - pattern);
    strcat(res, part);
    strcat(res, close + 1);
    return res;
}

static int count_parts(char **parts)
{
    int i = 0;

    while (parts[i])
        i++;
    return i;
}

static char *find_close(char *open)
{
    if (!open)
        return NULL;
    return strchr(open, '}');
}

static char *extract_inside(char *open, char *close)
{
    char *inside = my_strndup(open + 1, close - open - 1);

    if (!inside)
        return NULL;
    return inside;
}

static char **alloc_res(int count)
{
    char **res = malloc(sizeof(char *) * (count + 1));

    if (!res)
        return NULL;
    return res;
}

static void fill_res(char **res, char **parts, brace_t *ctx)
{
    int i = 0;

    while (parts[i]) {
        res[i] = build_pattern(
            ctx->pattern,
            ctx->open,
            ctx->close,
            parts[i]
        );
        if (!res[i])
            return;
        i++;
    }
    res[i] = NULL;
}

static char **build_expand(brace_t *ctx, char *inside)
{
    char **parts;
    char **res;
    int count;

    parts = my_explode(inside, ",");
    if (!parts)
        return NULL;
    count = count_parts(parts);
    res = alloc_res(count);
    if (!res)
        return free_str_arr(parts), NULL;
    fill_res(res, parts, ctx);
    free_str_arr(parts);
    return res;
}

char **brace_expand(const char *pattern)
{
    brace_t ctx;
    char *inside;
    char **res;

    ctx.pattern = pattern;
    ctx.open = strchr(pattern, '{');
    ctx.close = find_close(ctx.open);
    if (!ctx.open)
        return NULL;
    if (!ctx.close)
        return NULL;
    inside = extract_inside(ctx.open, ctx.close);
    if (!inside)
        return NULL;
    res = build_expand(&ctx, inside);
    free(inside);
    return res;
}
