/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handling.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyukang <kyukang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 14:56:16 by kyukang           #+#    #+#             */
/*   Updated: 2024/10/14 17:21:19 by kyukang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*static void	is_pipe_redir(t_token *current, char **our_env)
{
	if (current->type == PIPE)
	{
		handle_pipe(current, current->next, our_env);
		return ;
	}
	if (current->type == TRUNC || current->type == APPEND
		|| current->type == INPUT)
	{
		//handle_redirection(tokens, current->next->str, current->type);
		return ;
	}
}*/

static int	is_external_command(char *cmd, char **our_env)
{
	char	*path_env;
	char	**paths;
	char	full_path[1024];
	int		i;

	path_env = NULL;
	i = 0;
	if (ft_strcmp(cmd, "env") == 0)
		return (0);
	while (our_env[i])
	{
		if (ft_strncmp(our_env[i], "PATH=", 5) == 0)
		{
			path_env = our_env[i] + 5;
			break ;
		}
		i++;
	}
	if (!path_env)
		return (0);
	paths = ft_split(path_env, ':');
	i = 0;
	while (paths[i] != NULL)
	{
		ft_strcpy(full_path, paths[i]);
		ft_strcat(full_path, "/");
		ft_strcat(full_path, cmd);
		if (access(full_path, X_OK) == 0)
		{
			free_split(paths);
			return (1);
		}
		i++;
	}
	free_split(paths);
	return (0);
}

static int	is_internal_command(char *cmd)
{
	if (ft_strcmp(cmd, "cd") == 0)
		return (1);
	if (ft_strcmp(cmd, "pwd") == 0)
		return (1);
	if (ft_strcmp(cmd, "echo") == 0)
		return (1);
	if (ft_strcmp(cmd, "export") == 0)
		return (1);
	if (ft_strcmp(cmd, "unset") == 0)
		return (1);
	if (ft_strcmp(cmd, "env") == 0)
		return (1);
	if (ft_strcmp(cmd, "exit") == 0)
		return (1);
	return (0);
}

void	handle_tokens(t_token *tokens, char **our_env)
{
	int	pipe_count;
	int	**pipe_fds;
	pid_t	*pids;
	t_token	*current;
	int	i;

	pipe_count = count_pipes(tokens);
	pipe_fds = malloc(sizeof(int *) * pipe_count);
	pids = malloc(sizeof(pid_t) * (pipe_count + 1));
	current = tokens;
	i = 0;
	while (i < pipe_count)
	{
		pipe_fds[i] = malloc(sizeof(int) * 2);
		if (pipe(pipe_fds[i]) == -1)
		{
			perror("pipe");
			//free allocated memory
			return ;
		}
		i++;
	}
	i = 0;
	while (i <= pipe_count)
	{
		t_token *cmd_end = current;
		while (cmd_end && cmd_end->type != PIPE)
			cmd_end = cmd_end->next;
		if (is_internal_command(current->str) && pipe_count == 0)
			execute_command(current, cmd_end, our_env);
		else
		{
			pids[i] = fork();
			if (pids[i] == -1)
			{
				perror("fork");
				//free allocated memory
				return ;
			}
			else if (pids[i] == 0)
			{
				if (i > 0)
					dup2(pipe_fds[i-1][0], STDIN_FILENO);
				if (i < pipe_count)
					dup2(pipe_fds[i][1], STDOUT_FILENO);

				int j = 0;
				while (j < pipe_count)
				{
					close(pipe_fds[j][0]);
					close(pipe_fds[j][1]);
					j++;
				}
				execute_command(current, cmd_end, our_env);
				//exit(EXIT_SUCCESS);
			}
		}
		if (cmd_end)
			current = cmd_end->next;
		else
			current = NULL;
		i++;
	}
	i = 0;
	while (i < pipe_count)
	{
		close(pipe_fds[i][0]);
		close(pipe_fds[i][1]);
		free(pipe_fds[i]);
		i++;
	}
	free(pipe_fds);
	i = 0;
	while (i <= pipe_count)
	{
		waitpid(pids[i], NULL, 0);
		i++;
	}
	free(pids);
}

void	execute_command(t_token *start, t_token *end, char **our_env)
{
	int	fd_in = STDIN_FILENO;
	int	fd_out = STDOUT_FILENO;
	int	arg_count;

	setup_redirect(start, end, &fd_in, &fd_out);
	if (start && start->type == CMD)
	{
		if (is_external_command(start->str, our_env))
		{
			arg_count = count_tokens_until(start, end);
			execute_external_command(start, arg_count, our_env, fd_in, fd_out);
		}
		else if (is_internal_command(start->str))
		{
			t_command *cmd = init_internal_command(start, our_env);
			if (!cmd)
				return;
			cmd->fd_in = fd_in;
			cmd->fd_out = fd_out;
			execute_internal_commands(cmd, &our_env);
			free_command(cmd);
		}
	}
	if (fd_in != STDIN_FILENO)
		close(fd_in);
	if (fd_out != STDOUT_FILENO)
		close(fd_out);
}

int	count_tokens_until(t_token *start, t_token *end)
{
	int	count;
    	t_token	*current;

	count = 0;
    	current = start;
	while (current != end && current != NULL)
	{
		count++;
		current = current->next;
	}
	return (count);
}


/*void	execute_command(t_token *start, t_token *end, char **our_env)
{
	int	fd_in;
	int	fd_out;
	int	arg_count;

	fd_in = STDIN_FILENO;
	fd_out = STDOUT_FILENO;

	setup_redirect(start, end, &fd_in, &fd_out);
	if (start && start->type == CMD)
	{
		if (is_external_command(start->str, our_env))
		{
			if (end)
				arg_count = count_tokens_until(start, end);
			else
				arg_count = count_tokens_until(start, NULL);
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
			execute_external_command(start, arg_count, our_env, fd_in, fd_out);
		}
		else if (is_internal_command(start->str))
		{
			t_command *cmd = init_internal_command(start, our_env);
			if (!cmd)
				return;
			cmd->fd_in = fd_in;
			cmd->fd_out = fd_out;
			execute_internal_commands(cmd, &our_env);
			free_command(cmd);
		}
	}
}*/