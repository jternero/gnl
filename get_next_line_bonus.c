/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jternero <jternero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/11 15:18:20 by jternero          #+#    #+#             */
/*   Updated: 2022/12/17 14:58:04 by jternero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*necesito un timeout de 14 segundos para 
que con buffersize1 acabe el test 9 del paco 
paco --timeout 14*/

#include "get_next_line.h"

size_t	ft_strlcpy(char *dst, char *src, size_t dstsize)
{
	unsigned int	i;
	unsigned int	x;

	i = 0;
	if (dst == NULL || src == NULL)
		return (0);
	x = ft_strlen(src);
	if (dstsize != 0)
	{
		while (src[i] != '\0' && i < dstsize - 1)
		{
			dst[i] = src[i];
			i++;
		}
		dst[i] = '\0';
	}
	return (x);
}

char	*ft_substr(char *s, unsigned int start, size_t len)
{
	char	*str;

	if (ft_strlen(s) == 0)
		return (NULL);
	if (start > ft_strlen(s))
	{
		len = 0;
		start = 0;
	}
	if (start + len > ft_strlen(s) || len == 0)
		len = ft_strlen(s) - start;
	if (len > ft_strlen(s))
		len = ft_strlen(s + start);
	str = (char *)malloc(sizeof(char) * (len + 1));
	if (str == NULL)
		return (NULL);
	ft_strlcpy(str, (s + start), len + 1);
	return (str);
}

char	*ft_read_and_stash(char *stash, int fd, int *nbrc)
{
	char	*buf;
	char	*new_stash;

	buf = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (buf == NULL)
		return (stash);
	*nbrc = read(fd, buf, BUFFER_SIZE);
	if (*nbrc == -1)
		new_stash = NULL;
	else if (*nbrc > 0)
	{
		buf[*nbrc] = '\0';
		new_stash = ft_strjoin(stash, buf);
	}
	else
	{
		new_stash = ft_strjoin(stash, "");
	}
	free (buf);
	return (new_stash);
}

char	*ft_get_and_free_stash(char **stash, int len, int set_stash_null)
{
	char	*line;

	line = ft_substr(*stash, 0, len);
	free(*stash);
	if (set_stash_null)
		*stash = NULL;
	return (line);
}

char	*get_next_line(int fd)
{
	static char	*stash[4096];
	int			line_len;
	int			nbrc;
	char		*old_stash;

	nbrc = 0;
	while (fd >= 0 && BUFFER_SIZE > 0 && nbrc >= 0)
	{
		old_stash = stash[fd];
		if (ft_strchr(stash[fd], '\n'))
		{
			line_len = ft_strlen_n(stash[fd]) + 1;
			stash[fd] = ft_substr(old_stash, line_len, 0);
			return (ft_get_and_free_stash(&old_stash, line_len, 0));
		}
		else
		{
			stash[fd] = ft_read_and_stash(old_stash, fd, &nbrc);
			free(old_stash);
			if (nbrc == 0)
				return (ft_get_and_free_stash(&stash[fd], 0, 1));
		}
	}
	return (NULL);
}
