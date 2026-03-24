/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnayel <hnayel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 16:47:44 by hnayel            #+#    #+#             */
/*   Updated: 2026/03/17 12:17:08 by hnayel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strdup(char *str)
{
	size_t	len;
	int		i;
	char	*new_string;

	i = 0;
	len = ft_strlen(str);
	new_string = malloc(sizeof(char) * len + 1);
	if (!new_string)
		return (NULL);
	while (str[i])
	{
		new_string[i] = str[i];
		i++;
	}
	new_string[i] = '\0';
	return (new_string);
}
