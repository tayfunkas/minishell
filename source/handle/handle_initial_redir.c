/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_initial_redir.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyukang <kyukang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 14:36:26 by tkasapog          #+#    #+#             */
/*   Updated: 2024/10/30 18:45:52 by kyukang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_initial_redir(t_master *master, int *fd_in)
{
	int	flags;
	int	fd;

	if (!master->token->cur || !master->token->cur->str)
		return (0);
	if (master->token->cur->str[0] == '>' || master->token->cur->str[0] == '<')
	{
		if (!master->token->cur->next)
		{
			write(2, "syntax error near unexpected token `newline'\n", 44);
			return (2);
		}
		if (master->token->cur->str[0] == '>')
		{
			flags = O_WRONLY | O_CREAT;
			flags |= (master->token->cur->str[1] == '>') ? O_APPEND : O_TRUNC;
			fd = open(master->token->cur->next->str, flags, 0644);
			if (fd == -1)
			{
				perror("open");
				return (1);
			}
			close(fd);
		}
		else if (master->token->cur->str[0] == '<')
		{
			if (master->token->cur->str[1] == '<')
				execute_redir_heredoc(master->token->cur, fd_in);
			else
			{
				fd = open(master->token->cur->next->str, O_RDONLY);
				if (fd == -1)
				{
					write(2, "minishell: ", 11);
					write(2, master->token->cur->next->str, ft_strlen(master->token->cur->next->str));
					write(2, " : No such file or directory\n", 28);
					return (1);
				}
				if (*fd_in != STDIN_FILENO)
					close(*fd_in);
				*fd_in = fd;
			}
		}
		master->token->cur = master->token->cur->next->next;
		return (handle_initial_redir(master, fd_in));
	}
	return (0);
}
