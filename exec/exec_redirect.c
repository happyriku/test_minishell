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


void	do_heredoc(t_node *redirect)
{
	char	*line;
	char	*buf;
	int		n;

	while (1)
	{
		line = readline(">");
		if (!line || strcmp(line, redirect->delimiter) == 0)
		{
			if (line)
				free(line);
			break ;
		}
		write(redirect->pipefd[1], line, strlen(line));
		write(redirect->pipefd[1], "\n", 1);
		free(line);
	}
	close(redirect->pipefd[1]);
	return ;
}

void	do_redirect(t_node *redirect)
{
	int		fd;
	char	*buf;

	buf = malloc(sizeof(char));
	if (!buf)
		fatal_error("error");
	if (!redirect)
		return ;
	if (redirect->kind == ND_HEREDOC)
	{
		do_heredoc(redirect);
		if (dup2(redirect->pipefd[0], redirect->std_fd) == -1)
			fatal_error("dup2");
		close(redirect->pipefd[0]);
		return ;
	}
	else
	{
		redirect->file_fd = open_file(redirect);
		if (dup2(redirect->file_fd, redirect->std_fd) == -1)
			fatal_error("dup2");
		close(fd);
	}
	do_redirect(redirect->next);
}
