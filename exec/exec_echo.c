#include "../include/minishell.h"

int	exec_echo(char **argv, t_node *node)
{
	int	i;

	i = 0;
	while (argv[++i])
	{
		write(STDOUT_FILENO, argv[i], strlen(argv[i]));
		if (argv[i + 1])
			write(STDOUT_FILENO, " ", 1);
	}
	printf("\n");
	return (free(argv), 1);
}
