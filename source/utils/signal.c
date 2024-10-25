/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kyukang <kyukang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 15:45:59 by kyukang           #+#    #+#             */
/*   Updated: 2024/10/25 19:53:58 by kyukang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_signal = 0;

void	sigint_handler(int sig)
{
	if (sig == SIGINT)
	{
		printf("You are in sigint handler\n");
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	heredoc_sigint_handler(int sig)
{
	if (sig == SIGINT)
	{
		printf("You are in heredoc sigint handler\n");
		write(1, "\n", 1);
		exit(1);
	}
}

void	setup_signal(void)
{
	struct termios	term;

	if (tcgetattr(0, &term) != 0)
		perror("tcgetattr");
	term.c_lflag &= ~ECHOCTL;
	if (tcsetattr(0, 0, &term) != 0)
		perror("tcsetattr");
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}
