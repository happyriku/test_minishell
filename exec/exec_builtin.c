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

void	exec_nonbuiltin(t_node *node)
{
	char	*path;

	path = search_path(node->args->arr[0]);
	if (!path)
	{	
		if (!strchr(node->args->arr[0], '/'))
		{
			printf("%s: command not found\n", node->args->arr[0]);
			return ;
		}
		path = node->args->arr[0];
	}
	if (execve(path, node->args->arr, environ) == -1)
		exit(EXIT_SUCCESS); //free(path) deleteしました
}

void	exec_builtin(t_node *node)
{
	if (strcmp(node->args->arr[0], "echo") == 0)
		exec_echo(node->args->arr, node);
}