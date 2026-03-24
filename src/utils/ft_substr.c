/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnayel <hnayel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 16:02:21 by hnayel            #+#    #+#             */
/*   Updated: 2026/03/17 12:16:50 by hnayel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

static size_t	get_len(char *s, size_t len)
{
	size_t	i;

	i = 0;
	while (s[i] && i < len)
		i++;
	return (i + 1);
}

char	*ft_substr(char *s, unsigned int start, size_t len)
{
	char	*tmp;
	size_t	i;
	size_t	s_len;

	s_len = ft_strlen(s);
	if (s_len <= start)
	{
		tmp = ft_strdup("");
		if (!tmp)
			return (NULL);
		return (tmp);
	}
	i = 0;
	tmp = malloc(sizeof(char) * get_len(&s[start], len));
	if (!tmp)
		return (NULL);
	while (s[start + i] && i < len)
	{
		tmp[i] = s[start + i];
		i++;
	}
	tmp[i] = '\0';
	return (tmp);
}
