#include "../include/minishell.h"

extern	char	**environ;

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
	close(redirect->file_fd);
	if (dup2(redirect->stash_fd, redirect->std_fd) == -1)
		fatal_error("dup2");
	close(redirect->stash_fd);
}

bool	is_builtin(t_token *token)
{
	
	if (strcmp(token->word, "echo") == 0)
		return (true);
	else if (strcmp(token->word, "exit") == 0)
		return (true);
	else
		return (false);
}

void	prepare_parent_pipe(t_node *node, int in_fd)
{
	if (node->redirect && node->redirect->kind == ND_HEREDOC)
	{
		close(node->redirect->pipefd[0]);
		close(node->redirect->pipefd[1]);
	}
	close(node->pfd[1]);
	if (in_fd != STDIN_FILENO)
		close(in_fd);
}

int	wait_pid(void)
{
	int		wstatus;
	int		status;
	pid_t	res;

	res = wait(&wstatus);
	printf("res : %d\n", res);
	if (res < 0)
	{
		if (errno != EINTR && errno != ECHILD)
			fatal_error("wait");
	}
	status = WEXITSTATUS(wstatus);
	return (status);
}

int	exec_command(t_node *node, int in_fd)
{
	int		pid;
	char	*path;
	int		wstatus;
	int		status;

	if (pipe(node->pfd) == -1)
	 	fatal_error("pipe");
	pid = fork();
	if (pid < 0)
		return (-1);
	else if (pid == 0)
	{
		reset_signals();
		prepare_child_pipe(node, in_fd);
		if (is_builtin(node->args))
			exit(exec_builtin(node));
		else
		{
			exec_nonbuiltin(node);
			exit(EXIT_SUCCESS);
		}
	}
	else
	{
		prepare_parent_pipe(node, in_fd);
		if (node->next)
			exec_command(node->next, node->pfd[0]);
		else
			close(node->pfd[0]);
		return (wait_pid());
	}
}
