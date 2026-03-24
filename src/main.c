/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnayel <hnayel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/24 20:34:55 by hnayel            #+#    #+#             */
/*   Updated: 2026/03/24 21:03:59 by hnayel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int ac, char **av, char **env)
{
	char	*input;
	t_token	*tokens;
	t_cmd	*cmds;

	(void)ac;
	(void)av;
	while (1)
	{
		input = readline("$> ");
		if (!input)
		{
			printf("exit\n");
			break ;
		}
		if (*input)
			add_history(input);
		tokens = tokenize_input(input);
		cmds = parse_tokens(tokens);
		exec_cmd(tokens, cmds, env);
		// print_tokens(tokens);
		// print_cmds(cmds);
		free_tokens(tokens);
		free_cmds(cmds);
		free(input);
	}
	return (0);
}