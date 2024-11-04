/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyukang <kyukang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 16:00:35 by kyukang           #+#    #+#             */
/*   Updated: 2024/11/04 17:58:53 by kyukang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_all(t_ctx *ctx, t_tok *tokens, t_cmd *cmdlist)
{
	free_context(ctx);
	free_tokens(tokens);
	free_command_list(cmdlist);
}

int	ft_exit(char **args, t_ctx *ctx, t_tok *tokens, t_cmd *cmdlist)
{
	int	status;

	status = 0;
	printf("exit\n");
	if (args[1] == NULL)
	{
		free_all(ctx, tokens, cmdlist);
		exit(0);
	}
	if (!ft_isdigit_str(args[1]))
	{
		write(2, "minishell: exit: ", 17);
		write(2, args[1], ft_strlen(args[1]));
		write(2, ": numeric argument required\n", 28);
		free_all(ctx, tokens, cmdlist);
		exit(2);
	}
	if (args[2] != NULL)
	{
		write(2, "minishell: exit: too many arguments\n", 36);
		return (1);
	}
	status = ft_atoi(args[1]) % 256;
	free_all(ctx, tokens, cmdlist);
	exit(status);
}
