/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_commands.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkasapog <tkasapog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 15:46:35 by tkasapog          #+#    #+#             */
/*   Updated: 2024/10/17 14:01:00 by tkasapog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h" 

char	*expand_var(char *token, char **our_env, char quote)
{
	char	*env_var;
	int		i;

	i = 0;
	if (token[0] == '$' && quote != '\'')
	{
		env_var = token + 1;
		while (our_env[i])
		{
			if (ft_strncmp(our_env[i], env_var, ft_strlen(env_var)) == 0 
				&& our_env[i][ft_strlen(env_var)] == '=')
				return (ft_strdup(our_env[i] + ft_strlen(env_var) + 1));
			i++;
		}
		return (ft_strdup(""));
	}
	return (ft_strdup(token));
}

void	prep_args(t_token *tokens, int token_count, char **args, char **our_env)
{
	t_token	*current;
	int		i;

	current = tokens;
	i = 0;
	while (current && i < token_count)
	{
		if (current->type == CMD || current->type == ARG)
		{
			args[i] = expand_var(current->str, our_env, current->quote);
			i++;
		}
		else if (current->type == TRUNC || current->type == APPEND 
			|| current->type == INPUT || current->type == HEREDOC)
			current = current->next;
		current = current->next;
	}
	args[i] = NULL;
}

void	free_external_c(char *cmd_path, char **args, int token_count)
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

void	execute_external_c(t_token *tokens, int token_count, t_command *cmd)
{
	char		*cmd_path;
	char		**args;
	const char	*error_message;

	cmd_path = find_cmd_path(tokens->str, cmd->env);
	if (cmd_path == NULL)
	{
		error_message = "Command not found: ";
		write(STDERR_FILENO, error_message, ft_strlen(error_message));
		write(STDERR_FILENO, tokens->str, ft_strlen(tokens->str));
		write(STDERR_FILENO, "\n", 1);
		return ;
	}
	args = allocate_args(token_count);
	if (args == NULL)
	{
		free(cmd_path);
		return ;
	}
	prep_args(tokens, token_count, args, cmd->env);
	fork_and_execute(cmd, cmd_path, args);
	free_external_c(cmd_path, args, token_count);
}
