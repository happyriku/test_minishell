#include "../include/minishell.h"

int	open_file(t_node *redirect)
{
	int	fd;

	if (redirect->kind == ND_REDIRECT_OUT)
		fd = open(redirect->filename,
				O_CREAT | O_WRONLY | O_TRUNC, 0644);
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

int	do_heredoc(t_node *redirect, char *kind)
{
	char	*line;
	int		pipefd[2];
	char	buf;

	while (1)
	{
		line = readline(">");
		if (!line)
			break ;
		if (strcmp(kind, CHILD) == 0 && strcmp(line, redirect->delimiter) == 0)
		{
			close(redirect->pipefd[1]);
			while (read(redirect->pipefd[0], &buf, 1) > 0)
				write(STDOUT_FILENO, &buf, 1);
			close(redirect->pipefd[0]);
			exit(0);
		}
		close(redirect->pipefd[0]);
		while (*line)
			write(redirect->pipefd[1], line++, 1);
	}
	close(pipefd[0]);
	close(pipefd[1]);
	return (0);
}
