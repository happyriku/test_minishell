#include "../include/minishell.h"

extern char **environ;

int	open_file(t_node *redirect)
{
	int	fd;

	printf("redirect->kind : %d\n", redirect->kind);
	if (redirect->kind == ND_REDIRECT_OUT)
	{
		fd = open(redirect->filename,
				O_CREAT | O_WRONLY | O_TRUNC, 0644);
	}
	else if (redirect->kind == ND_REDIRECT_IN)
		fd = open(redirect->filename, O_RDONLY);
	else if (redirect->kind == ND_REDIRECT_APPEND)
		fd = open(redirect->filename, O_CREAT | O_WRONLY | O_APPEND, 0644);
	if (fd < 0)
		fatal_error("open");
	return (fd);
}

void	do_redirect(t_node *redirect)
{
	int	fd;

	if (!redirect)
		return ;
	redirect->file_fd = open_file(redirect);
	if (dup2(redirect->file_fd, redirect->std_fd) == -1)
		fatal_error("dup2");
	close(fd);
	do_redirect(redirect->next);
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

	argv = (char **)malloc(sizeof(char *) * (ft_lstsize(node->args) + 1));
	if (!argv)
		return (MALLOC_ERROR);
	setup_argv(argv, node->args);
	if (strncmp(argv[0], "exit", 4) == 0)
	{
		printf("exit\n");
		return (free(argv), -1);
	}
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
			return (exec_echo(argv, node));
		else
			if (execve(path, argv, environ) == -1)
				return (free(argv), 0);
	}
	else
	{
		wait(&status);
		free(argv);
		return (!WIFEXITED(status));
	}
	return (0);
}