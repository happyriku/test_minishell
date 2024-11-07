#include "../include/minishell.h"

void	create_enviroment_variable(char	*str)
{
	char	*key;
	char	*value;

	key = ft_strtok(str, "=");
	value = ft_strtok(NULL, "=");
	if (!key || !value)
	{
		printf("Invalid syntax. Use: export VAR=value\n");
		exit(1);
	}
	if (setenv(key, value, 1) == -1)
		fatal_error("setenv error");
}

int	builtin_export(char	**args)
{
	int	argc;
	int	i;

	argc = count_args(args);
	if (!(argc == 1 || argc == 2))
	{
		i = 1;
		while (args[++i])
			printf("-bash: export: %s: not a valid identifier\n", args[i]);
		exit(1);
	}
	if (args[0] && args[1])
		create_enviroment_variable(args[1]);
	else
		printf("do");
	return (0);
}