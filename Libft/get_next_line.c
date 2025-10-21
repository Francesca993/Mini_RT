/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmontini <fmontini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 09:54:57 by fmontini          #+#    #+#             */
/*   Updated: 2025/02/17 14:31:03 by fmontini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUFFER_SIZE
# define BUFFER_SIZE 42
#endif

#include "libft.h"

static int	read_and_store(int fd, char **storage)
{
	char	*buffer;
	ssize_t	bytes_read;
	char	*temp;

	buffer = (char *)malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (!buffer)
		return (-1);
	bytes_read = read(fd, buffer, BUFFER_SIZE);
	if (bytes_read < 0)
	{
		free(buffer);
		return (-1);
	}
	buffer[bytes_read] = '\0';
	temp = ft_strjoin(*storage, buffer);
	free(buffer);
	if (!temp)
		return (-1);
	free(*storage);
	*storage = temp;
	if (!*storage)
		return (-1);
	return (bytes_read);
}

static char	*extract_line(char **storage)
{
	char	*line;
	char	*new_storage;
	size_t	i;

	i = 0;
	while ((*storage)[i] != '\0' && (*storage)[i] != '\n')
		i++;
	if ((*storage)[i] == '\n')
		line = ft_substr(*storage, 0, i + 1);
	else
		line = ft_substr(*storage, 0, i);
	if (line == NULL)
		return (NULL);
	if ((*storage)[i] == '\n')
		new_storage = ft_strdup(*storage + i + 1);
	else
		new_storage = NULL;
	free(*storage);
	*storage = new_storage;
	return (line);
}

char	*get_next_line(int fd)
{
	static char	*storage;
	int			bytes_read;

	if (fd < 0 || BUFFER_SIZE < 0)
		return (NULL);
	if (!storage)
		storage = ft_strdup("");
	if (!storage)
		return (NULL);
	bytes_read = 1;
	while (bytes_read > 0 && !ft_strchr(storage, '\n'))
		bytes_read = read_and_store(fd, &storage);
	if (bytes_read < 0 || (bytes_read == 0 && (!storage || *storage == '\0')))
	{
		free(storage);
		storage = NULL;
		return (NULL);
	}
	return (extract_line(&storage));
}
