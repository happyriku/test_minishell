#include "../include/minishell.h"

bool	is_echo_option(char *option)
{
	if (!option)
		return (false);
	if (strcmp(option, "-n") == 0)
		return (true);
	return (false);
}

int	builtin_echo(char **argv)
{
	int	i;

	if (is_echo_option(argv[1]))
	{
		i = 1;
		while (argv[++i])
		{
			write(STDOUT_FILENO, argv[i], strlen(argv[i]));
			if (argv[i + 1])
				write(STDOUT_FILENO, " ", 1);
		}
		return (0);
	}
	i = 0;
	while (argv[++i])
	{
		write(STDOUT_FILENO, argv[i], strlen(argv[i]));
		if (argv[i + 1])
			write(STDOUT_FILENO, " ", 1);
	}
	printf("\n");
	return (0);
}
