/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_others.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyukang <kyukang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 18:01:46 by kyukang           #+#    #+#             */
/*   Updated: 2024/11/05 01:22:44 by kyukang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_redir_trunc(t_tok *current, int *fd_out)
{
	if (*fd_out != STDOUT_FILENO)
		close(*fd_out);
	*fd_out = open(current->next->str, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (*fd_out == -1)
	{
		perror("open");
		g_signal = 1;
		return (1);
	}
	return (0);
}

int	execute_redir_append(t_tok *current, int *fd_out)
{
	if (*fd_out != STDOUT_FILENO)
		close(*fd_out);
	*fd_out = open(current->next->str, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (*fd_out == -1)
	{
		write(2, "minishell: ", 11);
		write(2, current->next->str, ft_strlen(current->next->str));
		write(2, ": Permission denied\n", 20);
		g_signal = 1;
		return (1);
	}
	return (0);
}

int	execute_redir_input(t_tok *current, int *fd_in)
{
	if (*fd_in != STDIN_FILENO)
		close(*fd_in);
	*fd_in = open(current->next->str, O_RDONLY);
	if (*fd_in == -1)
	{
		write(2, "minishell: ", 11);
		write(2, current->next->str, ft_strlen(current->next->str));
		write(2, ": No such file or directory\n", 28);
		g_signal = 1;
		return (1);
	}
	return (0);
}
