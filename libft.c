/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paude-so <paude-so@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 04:08:38 by paude-so          #+#    #+#             */
/*   Updated: 2024/12/14 04:17:43 by paude-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

size_t ft_strlen(const char *s)
{
    size_t i;

    i = 0;
    while (s[i])
        i++;
    return (i);
}

char *ft_strchr(const char *s, int c)
{
    while (*s)
    {
        if (*s == (char)c)
            return ((char *)s);
        s++;
    }
    if (*s == (char)c)
        return ((char *)s);
    return (NULL);
}

char *ft_strrchr(const char *s, int c)
{
    char *last;

    last = NULL;
    while (*s)
    {
        if (*s == (char)c)
            last = (char *)s;
        s++;
    }
    if (*s == (char)c)
        return ((char *)s);
    return (last);
}

int ft_strcmp(const char *s1, const char *s2)
{
    while (*s1 && *s2 && *s1 == *s2)
    {
        s1++;
        s2++;
    }
    return ((unsigned char)*s1 - (unsigned char)*s2);
}

char *ft_strdup(const char *s)
{
    char *dup;
    size_t i;

    dup = malloc(sizeof(char) * (ft_strlen(s) + 1));
    if (!dup)
        return (NULL);
    i = 0;
    while (s[i])
    {
        dup[i] = s[i];
        i++;
    }
    dup[i] = '\0';
    return (dup);
}
