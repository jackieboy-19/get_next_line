/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbrito <jbrito@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/13 09:45:44 by jbrito            #+#    #+#             */
/*   Updated: 2026/11/17 15:20:18 by jbrito           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*ft_next_line(char *s)
{
	int		i;
	char	*line;

	i = 0;
	if (!s || s[i] == '\0')
		return (NULL);
	while (s[i] && s[i] != '\n')
		i++;
	if (s[i] == '\n')
		i++;
	line = malloc(sizeof(char) * (i + 1));
	if (line == NULL)
		return (NULL);
	i = 0;
	while (s[i] != '\0' && s[i] != '\n')
	{
		line[i] = s[i];
		i++;
	}
	if (s[i] == '\n')
		line[i++] = '\n';
	line[i] = '\0';
	return (line);
}

char	*ft_get_rest(char *box)
{
	char	*str;
	char	*rest;
	size_t	i;

	str = ft_strchr(box, '\n');
	if (!str || str[1] == '\0')
	{
		free(box);
		return (NULL);
	}
	rest = malloc(sizeof(char) * (ft_strlen(str + 1) + 1));
	if (!rest)
	{
		free(box);
		return (NULL);
	}
	i = 0;
	while (str[i + 1])
	{
		rest[i] = str[i + 1];
		i++;
	}
	rest[i] = '\0';
	free(box);
	return (rest);
}

char	*ft_clean(char *a, char *b)
{
	free(a);
	free(b);
	return (NULL);
}

char	*ft_read(int fd, char *str)
{
	char		*buffer;
	int			bytes;

	buffer = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (buffer == NULL)
		return (ft_clean(str, NULL));
	bytes = 1;
	while (!(ft_strchr(str, '\n')) && bytes > 0)
	{
		bytes = read(fd, buffer, BUFFER_SIZE);
		if (bytes == -1)
			return (ft_clean(buffer, str));
		buffer[bytes] = '\0';
		str = ft_strjoin(str, buffer);
		if (!str)
			return (ft_clean(buffer, NULL));
	}
	free(buffer);
	return (str);
}

char	*get_next_line(int fd)
{
	static char	*str;
	char		*line;
	int			pending;

	if (BUFFER_SIZE <= 0 || fd < 0)
		return (NULL);
	str = ft_read(fd, str);
	line = ft_next_line(str);
	if (!line)
	{
		free(str);
		str = NULL;
		return (NULL);
	}
	pending = (ft_strlen(str) > ft_strlen(line));
	str = ft_get_rest(str);
	if (pending && !str)
		return (ft_clean(line, NULL));
	return (line);
}
