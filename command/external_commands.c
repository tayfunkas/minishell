/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_commands.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyukang <kyukang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 17:50:18 by kyukang           #+#    #+#             */
/*   Updated: 2024/10/14 18:33:13 by kyukang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*expand_var(char *token, char **our_env)
{
	char	*env_var;
	int		i;

	i = 0;
	if (token[0] == '$')
	{
		env_var = token + 1;
		while (our_env[i])
		{
			if (ft_strncmp(our_env[i], env_var, ft_strlen(env_var)) == 0 && our_env[i][ft_strlen(env_var)] == '=')
				return ft_strdup(our_env[i] + ft_strlen(env_var) + 1);
			i++;
		}
		return (ft_strdup(""));
	}
	return (ft_strdup(token));
}

void	prepare_args(t_token *tokens, int token_count, char **args, char **our_env)
{
	t_token *current;
	int		i;

	current = tokens;
	i = 0;
	while (current && i < token_count)
	{
		if (current->type == CMD || current->type == ARG)
		{
			args[i] = expand_var(current->str, our_env);
			i++;
		}
		else if (current->type == TRUNC || current->type == APPEND || current->type == INPUT || current->type == HEREDOC)
			current = current->next;
		current = current->next;
	}
	args[i] = NULL;
}

void	handle_fork(pid_t pid, char *cmd_path, char **args, char **our_env)
{
	int	status;

	if (pid == 0)
	{
		if (execve(cmd_path, args, our_env) == -1)
			perror("Error from execve function\n");
		exit(EXIT_FAILURE);
	}
	else if (pid < 0)
		perror("Error from fork\n");
	else
		waitpid(pid, &status, 0);
}

void	free_external_commands(char *cmd_path, char **args, int token_count)
{
	int	i;

	free(cmd_path);
	i = 0;
	while (i < token_count)
	{
		free(args[i]);
		i++;
	}
	free(args);
}

void	execute_external_command(t_token *tokens, int token_count, char **our_env, int fd_in, int fd_out)
{
	pid_t	pid;
	char	*cmd_path;
	char	**args;

	cmd_path = find_cmd_path(tokens->str, our_env);
	if (cmd_path == NULL)
	{
		fprintf(stderr, "Command not found: %s\n", tokens->str);
		return;
	}
	args = (char **)malloc(sizeof(char *) * (token_count + 1));
	if (args == NULL)
	{
		perror("malloc failed");
		free(cmd_path);
		return;
	}
	prepare_args(tokens, token_count, args, our_env);
	int	parent_in = dup(STDIN_FILENO);
	int	parent_out = dup(STDOUT_FILENO);
	pid = fork();
	if (pid == 0)
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
		execve(cmd_path, args, our_env);
		perror("execve");
		exit(EXIT_FAILURE);
	}
	else if (pid > 0)
	{
		int status;
		if (fd_in != STDIN_FILENO)
			close(parent_in);
		if (fd_out != STDOUT_FILENO)
			close(parent_out);
		waitpid(pid, &status, 0);
		dup2(parent_in, STDIN_FILENO);
		dup2(parent_out, STDOUT_FILENO);
		close(parent_in);
		close(parent_out);
	}
	else
	{
    	perror("fork");
	}
	free_external_commands(cmd_path, args, token_count);
}
