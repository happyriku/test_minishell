#include "../include/minishell.h"

extern	char	**environ;

int	get_size(t_token *args)
{
	t_token	*tmp;
	int		count;

	tmp = args;
	count = 0;
	while (tmp && tmp->kind != TK_EOF)
	{
		tmp = tmp->next;
		count++;
	}
	return (count);
}

char	**create_args(t_token *token)
{
	char	**args;
	size_t	len;
	int		size;
	int		i;

	size = get_size(token);
	args = (char **)malloc(sizeof(char *) * (size + 1));
	if (!args)
		fatal_error("mallo");
	i = -1;
	while (token && token->kind != TK_EOF)
	{
		args[++i] = (char *)malloc(sizeof(char) * strlen(token->word) + 1);
		if (!args)
			fatal_error("malloc");
		ft_strncpy(args[i], token->word, strlen(token->word) + 1);
		token = token->next;
	}
	args[++i] = NULL;
	return (args);
}

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
	char	**args;
	
	if (node->redirect)
		do_redirect(node->redirect);
	args = create_args(node->args);
	path = search_path(args[0]);
	if (!path)
	{	
		if (!strchr(args[0], '/'))
		{
			printf("%s: command not found\n", args[0]);
			exit(127);
		}
		path = args[0];
	}
	if (execve(path, args, environ) == -1)
		exit(EXIT_SUCCESS); //free(path) deleteしました
}

int	exec_exit(t_node *node)
{
	printf("exit\n");
	g_info.last_status = 0;
	exit(g_info.last_status);
}

int	exec_builtin(t_node *node)
{
	char	**args;
	int		status;

	if (node->redirect)
		do_redirect(node->redirect);
	args = create_args(node->args);
	if (strcmp(args[0], "echo") == 0)
		status = exec_echo(args, node);
	else if (strcmp(args[0], "exit") == 0)
		status = exec_exit(node);
	if (node->redirect)
		reset_redirect(node->redirect);
	return (status);
}