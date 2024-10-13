#include "../include/minishell.h"

extern	char	**environ;

char	*search_path(char *input)
{
	char	*value;
	char	*path;
	char	*pos;
	int		path_len;

	value = getenv("PATH");
	if (!value)
		return (NULL);
	while (*value)
	{
		pos = strchr(value, ':');
		if (!pos)
			path_len = strlen(value);
		else
			path_len = strlen(value) - strlen(pos);
		path = (char *)malloc(sizeof(char) * (path_len + strlen(input) + 2));
		if (!path)
			return (NULL);
		ft_strncpy(path, value, path_len);
		ft_strlcat(path, "/", PATH_MAX);
		ft_strlcat(path, input, PATH_MAX);
		if (access(path, X_OK) == 0)
			return (path);
		free(path);
		if (!pos)
			return (NULL);
		value = pos + 1;
	}
	return (NULL);
}

int	exec_command_parent(t_node *node)
{
	int	status;

	if (node->redirect && node->redirect->kind == ND_HEREDOC)
	{
		close(node->redirect->pipefd[0]);
		close(node->redirect->pipefd[1]);
	}
	if (wait(&status) == -1)
		fatal_error("wait");
	return (!WIFEXITED(status));
}

int	exec_command(t_node *node, char **argv)
{
	int		pid;
	char	*path;

	pid = fork();
	if (pid < 0)
		return (free(argv), -1);
	else if (pid == 0)
	{
		if (node->redirect != NULL)
			do_redirect(node->redirect);	
		path = search_path(argv[0]);
		if (!path)
		{	
			if (!strchr(argv[0], '/'))
			{
				printf("%s: command not found\n", argv[0]);
				return (free(argv), 1);
			}
			path = argv[0];
		}
		if (strncmp(argv[0], "echo", 4) == 0 && argv[1] != NULL)
			return (free(path), exec_echo(argv, node));
		else
		{
			if (execve(path, argv, environ) == -1)
				return (free(argv), free(path), 0);
		}
	}
	else
		return (free(argv), exec_command_parent(node));
	// {
	// 	if (node->redirect && node->redirect->kind == ND_HEREDOC)
	// 	{
	// 		close(node->redirect->pipefd[0]);
	// 		close(node->redirect->pipefd[1]);
	// 	}
	// 	if (wait(&status) == -1)
	// 		fatal_error("wait");
	// 	free(argv);
	// 	return (!WIFEXITED(status));
	// }
	return (0);
}
