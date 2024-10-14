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

int	exec_command(t_node *node, char **argv, int in_fd)
{
	int		pid;
	char	*path;
	int		status;

	if (pipe(node->pfd) == -1)
		fatal_error("pipe");
	pid = fork();
	if (pid < 0)
		return (free(argv), -1);
	else if (pid == 0)
	{
		close(node->pfd[0]);
		if (in_fd != STDIN_FILENO)
		{
			if (dup2(in_fd, STDIN_FILENO) == -1)
			{
				fprintf(stderr, "dup2 error: %s\n", strerror(errno));
				exit(EXIT_FAILED);
			}
			close(in_fd);
		}
		else if (node->next)
		{
			if (dup2(node->pfd[1], STDOUT_FILENO) == -1)
			{
				fprintf(stderr, "dup2 error: %s\n", strerror(errno));
				exit(EXIT_FAILED);
			}
		}
		close(node->pfd[1]);
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
	{
		close(node->pfd[1]);
		if (in_fd != STDIN_FILENO)
			close(in_fd);
		if (node->next)
			exec_command(node->next, argv, node->pfd[0]);
		if (wait(&status) == -1)
			fatal_error("wait");
		return (free(argv), !WIFEXITED(status));
	}
	return (0);
}
