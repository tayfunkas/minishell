/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyukang <kyukang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 13:29:25 by kyukang           #+#    #+#             */
/*   Updated: 2024/11/04 13:29:57 by kyukang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strdup(char *s)
{
	char	*copy;
	size_t	len;
	size_t	i;

	len = 0;
	while (s[len] != '\0')
		len++;
	copy = (char *)malloc(len + 1);
	if (!copy)
		return (NULL);
	i = 0;
	while (i <= len)
	{
		copy[i] = s[i];
		i++;
	}
	return (copy);
}

char	*ft_strndup(const char *s1, size_t n)
{
	char	*copy;
	size_t	len;
	size_t	i;

	len = 0;
	while (s1[len] && len < n)
		len++;
	copy = (char *)malloc(len + 1);
	if (!copy)
		return (NULL);
	i = 0;
	while (i < len)
	{
		copy[i] = s1[i];
		i++;
	}
	copy[len] = '\0';
	return (copy);
}
