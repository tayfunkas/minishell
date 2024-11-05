/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyukang <kyukang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 14:32:05 by kyukang           #+#    #+#             */
/*   Updated: 2024/11/05 03:15:09 by kyukang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	duplicate_fds(int fd_in, int fd_out)
{
	if (fd_in != STDIN_FILENO)
	{
		dup2(fd_in, STDIN_FILENO);
		close(fd_in);
	}
	if (fd_out != STDOUT_FILENO)
	{
		dup2(fd_out, STDOUT_FILENO);
		close(fd_out);
	}
}

void	restore_fds(int parent_in, int parent_out)
{
	dup2(parent_in, STDIN_FILENO);
	dup2(parent_out, STDOUT_FILENO);
	close(parent_in);
	close(parent_out);
}

void	check_fds(t_cmd *cmd)
{
	if (cmd->fd_in != STDIN_FILENO)
		close(cmd->fd_in);
	if (cmd->fd_out != STDOUT_FILENO)
		close(cmd->fd_out);
}
