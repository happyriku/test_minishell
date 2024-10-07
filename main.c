/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rishibas <rishibas@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-09-08 04:22:00 by rishibas          #+#    #+#             */
/*   Updated: 2024-09-08 04:22:00 by rishibas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/minishell.h"

int	main(void)
{
	char	*input;
	int		res;

	while (1)
	{
		input = readline("minishell> ");
		if (!input)
		{
			printf("\nExit\n");
			break ;
		}
		if (*input)
			add_history(input);
		res = interpret(input);
		if (res == EXIT)
			return (free(input), EXIT_SUCCESS);
		if (res != 0)
		{
			rl_clear_history();
			return (free(input), EXIT_FAILED);
		}
	}
	return (EXIT_SUCCESS);
}