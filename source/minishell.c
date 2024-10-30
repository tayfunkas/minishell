/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyukang <kyukang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 14:39:13 by tkasapog          #+#    #+#             */
/*   Updated: 2024/10/30 18:57:20 by kyukang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_master	*init_master(char **envp)
{
	t_master	*master;

	master = malloc(sizeof(t_master));
	if (!master)
		return (NULL);
	master->token = NULL;
	master->cmd = malloc(sizeof(t_command));
	if (!master->cmd)
	{
		free(master);
		return (NULL);
	}
	ft_memset(master->cmd, 0, sizeof(t_command));
	master->ctx = malloc(sizeof(t_exec_context));
	if (!master->ctx)
	{
		free(master);
		return (NULL);
	}
	master->ctx->pipe_fds = NULL;
	master->ctx->pipe_count = 0;
	master->ctx->current_index = 0;
	master->ctx->last_status = 0;
	master->ctx->our_env = NULL;
	master->ctx->syntax_error = 0;
	master->ctx->our_env = copy_environment(envp);
	if (!master->ctx->our_env)
	{
		write(2, "Failied to copy environment\n", 28);
		free(master->ctx);
		free(master->cmd);
		free(master);
		return (NULL);
	}
	return (master);
}

int	main(int argc, char **argv, char **envp)
{
	t_master	*master;

	(void)argc;
	(void)argv;
	master = init_master(envp);
	if (!master)
	{
		write(2, "Failied to initiate struct\n", 27);
		free_master(master);
		return (1);
	}
	minishell(master);
	rl_clear_history();
	free_master(master);
	return (0);
}

static void	run_minishell(char *input, t_master *master)
{
	int		status;

	master->token = tokenize_inputs(input, 10);
	if (master->token)
	{
		expand_tokens(master);
		assign_token_types(master->token);
		if (check_syntax(master))
		{
			signal(SIGINT, SIG_IGN);
			status = handle_tokens(master);
			setup_signal();
			update_last_status(master->ctx, status);
		}
		else
			update_last_status(master->ctx, 2);
	}
}

void	minishell(t_master *master)
{
	char	*input;

	g_signal = 0;
	setup_signal();
	initialize_exit_status(master->ctx);
	while (1)
	{
		input = readline("minishell> ");
		if (input == NULL)
		{
			printf("exit\n");
			rl_clear_history();
			break ;
		}
		if (input[0])
		{
			add_history(input);
			run_minishell(input, master);
		}
		free(input);
	}
}

char	**copy_environment(char **envp)
{
	int		count;
	char	**new_env;
	int		i;

	count = 0;
	i = 0;
	while (envp[count] != NULL)
		count++;
	new_env = malloc(sizeof(char *) * (count + 1));
	if (!new_env)
		return (NULL);
	while (i < count)
	{
		new_env[i] = ft_strdup(envp[i]);
		if (!new_env[i++])
		{
			while (i-- >= 0)
				free(new_env[i]);
			free(new_env);
			return (NULL);
		}
	}
	new_env[count] = NULL;
	return (new_env);
}

void	free_environment(char **env)
{
	int	i;

	i = 0;
	while (env[i] != NULL)
	{
		free(env[i]);
		i++;
	}
	free(env);
}
