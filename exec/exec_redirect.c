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

void	do_heredoc(t_node *redirect)
{
	char	*line;

	while (1)
	{
		line = readline(">");
		if (!line)
			break ;
		break ;
	}
}