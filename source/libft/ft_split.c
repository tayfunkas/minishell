/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyukang <kyukang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 13:26:59 by kyukang           #+#    #+#             */
/*   Updated: 2024/11/04 13:27:50 by kyukang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_word(const char *s, char c)
{
	int	count;

	count = 0;
	while (*s)
	{
		while (*s && *s == c)
			s++;
		if (*s && *s != c)
		{
			count++;
			while (*s && *s != c)
				s++;
		}
	}
	return (count);
}

static char	*alloc_word(const char *start, int len)
{
	char	*word;
	int		i;

	word = malloc(len + 1);
	if (!word)
		return (NULL);
	i = 0;
	if (word)
	{
		while (i < len)
		{
			word[i] = start[i];
			i++;
		}
		word[len] = '\0';
	}
	return (word);
}

char	**ft_split(char const *s, char c)
{
	char		**result;
	int			i;
	const char	*word_start;

	if (!s)
		return (NULL);
	result = malloc((count_word(s, c) + 1) * sizeof(char *));
	if (!result)
		return (NULL);
	i = 0;
	while (*s)
	{
		while (*s == c)
			s++;
		word_start = s;
		while (*s && *s != c)
			s++;
		if (s > word_start)
		{
			result[i] = alloc_word(word_start, s - word_start);
			i++;
		}
	}
	result[i] = NULL;
	return (result);
}
