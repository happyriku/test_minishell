#include "../include/minishell.h"

int	count_args(char **args)
{
	int	i;
	int	count;

	i = -1;
	count = 0;
	while (args[++i])
		count++;
	return (count);
}

int	builtin_exit(char **args)
{
	int64_t	nbr;
	int		argc;

	printf("exit\n");
	argc = count_args(args);
	if (!(argc == 1 || argc == 2))
	{
		printf("bash: exit: too many arguments\n");
		exit(1);
	}
	if (args[1])
		g_info.last_status = ft_strtoll(args[1]);
	exit(g_info.last_status);
}
