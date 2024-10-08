/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasapog <tkasapog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 14:16:22 by tkasapog          #+#    #+#             */
/*   Updated: 2024/10/08 15:53:07 by tkasapog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	setup_redirect(t_token *start, t_token *end, int *fd_in, int *fd_out)
{
	t_token	*current;

	current = start;
	while (current != end && current != NULL)
	{
		if (current->type == INPUT)
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
		else if (current->type == TRUNC)
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
		else if (current->type == APPEND)
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
		current = current->next;
	}
}
