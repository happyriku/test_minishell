#include "../include/minishell.h"

extern	char	**environ;

// char	*search_path(char *input)
// {
// 	char	*value;
// 	char	*path;
// 	char	*pos;
// 	int		path_len;

// 	value = getenv("PATH");
// 	if (!value)
// 		return (NULL);
// 	while (*value)
// 	{
// 		pos = strchr(value, ':');
// 		if (!pos)
// 			path_len = strlen(value);
// 		else
// 			path_len = strlen(value) - strlen(pos);
// 		path = (char *)malloc(sizeof(char) * (path_len + strlen(input) + 2));
// 		if (!path)
// 			return (NULL);
// 		ft_strncpy(path, value, path_len);
// 		ft_strlcat(path, "/", PATH_MAX);
// 		ft_strlcat(path, input, PATH_MAX);
// 		if (access(path, X_OK) == 0)
// 			return (path);
// 		free(path);
// 		if (!pos)
// 			return (NULL);
// 		value = pos + 1;
// 	}
// 	return (NULL);
// }

void	prepare_child_pipe(t_node *node, int in_fd)
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
	if (node->next)
	{
		if (dup2(node->pfd[1], STDOUT_FILENO) == -1)
		{
			fprintf(stderr, "dup2 error: %s\n", strerror(errno));
			exit(EXIT_FAILED);
		}
	}
	close(node->pfd[1]);
}

void	reset_redirect(t_node *redirect)
{
	printf("---------\n");
	close(redirect->file_fd);
	if (dup2(redirect->stash_fd, redirect->std_fd) == -1)
		fatal_error("dup2");
	close(redirect->stash_fd);
}

bool	is_builtin(char **arg)
{
	if (strcmp(arg[0], "echo") == 0 && arg[1] != NULL)
		return (true);
	else if (strcmp(arg[0], "exit") == 0)
		return (true);
	else
		return (false);
}

int	exec_command(t_node *node, int in_fd)
{
	int		pid;
	char	*path;
	int		status;

	// if (pipe(node->pfd) == -1)
	// 	fatal_error("pipe");
	pid = fork();
	if (pid < 0)
		return (-1);
	else if (pid == 0)
	{
		//prepare_child_pipe(node, in_fd);
		if (node->redirect)
			do_redirect(node->redirect);
		if (is_builtin(node->args->arr))
		{
			exec_builtin(node);
			if (node->redirect)
				reset_redirect(node->redirect);
			exit(EXIT_SUCCESS);
		}
		else
		{
			exec_nonbuiltin(node);
			exit(EXIT_SUCCESS);
		}
		// path = search_path(node->args->arr[0]);
		// if (!path)
		// {	
		// 	if (!strchr(node->args->arr[0], '/'))
		// 	{
		// 		printf("%s: command not found\n", node->args->arr[0]);
		// 		exit(EXIT_SUCCESS);
		// 	}
		// 	path = node->args->arr[0];
		// }
		// if (strncmp(node->args->arr[0], "echo", 4) == 0 && node->args->arr[1] != NULL)
		// {
		// 	free(path);
		// 	exec_echo(node->args->arr, node);
		// 	exit(EXIT_SUCCESS);
		// }
		// if (!is_builtin(node->args->arr) && execve(path, node->args->arr, environ) == -1)
		// 	exit(EXIT_SUCCESS); //free(path) deleteしました
	}
	else
	{
		if (node->redirect && node->redirect->kind == ND_HEREDOC)
		{
			close(node->redirect->pipefd[0]);
			close(node->redirect->pipefd[1]);
		}
		//close(node->pfd[1]);
		// if (in_fd != STDIN_FILENO)
		// 	close(in_fd);
		// if (node->next)
		// 	exec_command(node->next, node->pfd[0]);
		// else
		// 	close(node->pfd[0]);
		if (wait(&status) == -1)
			fatal_error("wait");
		return (!WIFEXITED(status));
	}
	return (0);
}
