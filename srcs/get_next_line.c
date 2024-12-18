/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paude-so <paude-so@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 22:59:32 by paude-so          #+#    #+#             */
/*   Updated: 2024/12/13 23:02:07 by paude-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

static size_t	len_to_nl(char *str)
{
	size_t	len;

	if (!str)
		return (0);
	len = 0;
	while (str[len] && str[len] != '\n')
		len++;
	if (str[len] == '\n')
		len++;
	return (len);
}

static char	*copy_to_nxtline(char *nxt_line, char *str, size_t len)
{
	while (len--)
		*nxt_line++ = *str++;
	return (nxt_line);
}

static void	move_stash_left(char *stash, size_t stash_len)
{
	char	*after_nl;

	after_nl = stash + stash_len;
	while (*after_nl)
		*stash++ = *after_nl++;
	*stash = '\0';
}

static char	*merge_and_move(char *line, char *stash)
{
	char	*new_line;
	char	*temp;
	size_t	line_len;
	size_t	stash_len;

	line_len = len_to_nl(line);
	stash_len = len_to_nl(stash);
	new_line = malloc(line_len + stash_len + 1);
	if (!new_line)
		return (free(line), NULL);
	temp = new_line;
	temp = copy_to_nxtline(temp, line, line_len);
	temp = copy_to_nxtline(temp, stash, stash_len);
	*temp = '\0';
	free(line);
	move_stash_left(stash, stash_len);
	return (new_line);
}

char	*get_next_line(int fd)
{
	static char	stash[BUFFER_SIZE + 1];
	char		*result;
	ssize_t		read_count;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	result = NULL;
	read_count = 1;
	while (read_count > 0)
	{
		if (*stash)
		{
			result = merge_and_move(result, stash);
			if (result && result[len_to_nl(result) - 1] == '\n')
				break ;
		}
		read_count = read(fd, stash, BUFFER_SIZE);
		if (read_count == -1)
			return (free(result), NULL);
		stash[read_count] = '\0';
	}
	return (result);
}
