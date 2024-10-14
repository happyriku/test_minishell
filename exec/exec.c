#include "../include/minishell.h"

extern char **environ;

void	setup_argv(char **argv, t_token *args)
{
	int		i;
	t_token	*cur;

	cur = args;
	i = 0;
	while (cur->word)
	{
		argv[i] = cur->word;
		cur = cur->next;
		i++;
	}
	argv[i] = NULL;
}

int	exec(t_node *node)
{
	char	**argv;
	pid_t	pid;
	char	*path;
	int		i;
	int		status;

	// argv = (char **)malloc(sizeof(char *) * (ft_lstsize(node->args) + 1));
	// if (!argv)
	// 	return (MALLOC_ERROR);
	// setup_argv(argv, node->args);
	if (strncmp(node->args->arr[0], "exit", 4) == 0)
		return (free(argv), printf("exit\n"), -1);
	return (exec_command(node, STDIN_FILENO));
}
