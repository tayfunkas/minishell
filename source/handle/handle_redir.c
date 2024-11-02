/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyukang <kyukang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 18:06:21 by kyukang           #+#    #+#             */
/*   Updated: 2024/11/02 17:36:12 by kyukang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_initial_redir(t_token **current, int *fd_in)
{
	int	flags;
	int	fd;

	if (!*current || !(*current)->str)
		return (0);
	if ((*current)->str[0] == '>' || (*current)->str[0] == '<')
	{
		if (!(*current)->next)
		{
			write(2, "syntax error near unexpected token `newline'\n", 44);
			return (2);
		}
		if ((*current)->str[0] == '>')
		{
			flags = O_WRONLY | O_CREAT;
			flags |= ((*current)->str[1] == '>') ? O_APPEND : O_TRUNC;
			fd = open((*current)->next->str, flags, 0644);
			if (fd == -1)
			{
				perror("open");
				return (1);
			}
			close(fd);
		}
		else if ((*current)->str[0] == '<')
		{
			if ((*current)->str[1] == '<')
				execute_redir_heredoc(*current, fd_in);
			else
			{
				fd = open((*current)->next->str, O_RDONLY);
				if (fd == -1)
				{
					write(2, "minishell: ", 11);
					write(2, (*current)->next->str, ft_strlen((*current)->next->str));
					write(2, ": No such file or directory\n", 28);
					return (1);
				}
				if (*fd_in != STDIN_FILENO)
					close(*fd_in);
				*fd_in = fd;
			}
		}
		*current = (*current)->next->next;
		//return (handle_initial_redir(current, fd_in));
	}
	return (0);
}
