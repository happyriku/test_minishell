#include "../include/minishell.h"

extern char **environ;

int	builtin_env(void)
{
	char	**env;

	env = environ;
	while (*env)
	{
		printf("%s\n", *env);
		env++;
	}
	return (0);
}