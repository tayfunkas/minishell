/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_initial_redir.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasapog <tkasapog@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 14:36:26 by tkasapog          #+#    #+#             */
/*   Updated: 2024/10/30 14:36:38 by tkasapog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_initial_redir(t_token **current, int *fd_in)
{
	int	flags;
	int	fd;
	
	if (!*current || !(*current)->str)
		return 0;
	if ((*current)->str[0] == '>' || ((*current)->str[0] == '<' && (*current)->str[1] == '<'))
	{
		if (!(*current)->next)
		{
			write(2, "syntax error near unexpected token `newline'\n", 44);
			return 2;
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
		else if ((*current)->str[0] == '<' && (*current)->str[1] == '<')
			execute_redir_heredoc(*current, fd_in);
		*current = (*current)->next->next;
		return (handle_initial_redir(current, fd_in));
	}
	return (0);
}
