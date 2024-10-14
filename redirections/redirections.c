/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyukang <kyukang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 14:16:22 by tkasapog          #+#    #+#             */
/*   Updated: 2024/10/14 15:34:27 by kyukang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	execute_redir_input(t_token *current, int *fd_in)
{
	if (*fd_in != STDIN_FILENO)
		close(*fd_in);
	*fd_in = open(current->next->str, O_RDONLY);
	if (*fd_in == -1)
	{
		perror("open");
		exit(EXIT_FAILURE);
	}
}

static void	execute_redir_trunc(t_token *current, int *fd_out)
{
	if (*fd_out != STDOUT_FILENO)
		close(*fd_out);
	*fd_out = open(current->next->str, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (*fd_out == -1)
	{
		perror("open");
		exit(EXIT_FAILURE);
	}
}

static void	execute_redir_append(t_token *current, int *fd_out)
{
	if (*fd_out != STDOUT_FILENO)
		close(*fd_out);
	*fd_out = open(current->next->str, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (*fd_out == -1)
	{
		perror("open");
		exit(EXIT_FAILURE);
	}
}

static void	execute_redir_heredoc(t_token *current, int *fd_in)
{
	char	*line;
}

void	setup_redirect(t_token *start, t_token *end, int *fd_in, int *fd_out)
{
	t_token	*current;

	current = start;
	while (current != end && current != NULL)
	{
		if (current->type == INPUT)
			execute_redir_input(current, fd_in);
		else if (current->type == TRUNC)
			execute_redir_trunc(current, fd_out);
		else if (current->type == APPEND)
			execute_redir_append(current, fd_out);
		else if (current->type == HEREDOC)
			execute_redir_heredoc(current, fd_in);
		current = current->next;
	}
}
