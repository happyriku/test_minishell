// #include "../include/minishell.h"

// void	redirect_out(char	*pathname)
// {
// 	int	fd;

// 	printf("pathname : %s\n", pathname);
// 	fd = open(pathname, O_CREAT, O_RDONLY);
// 	if (fd == -1)
// 		fatal_error("open");
// 	if (dup2(fd, 1) == -1)
// 		fatal_error("dup2");
// }

// void	redirect(t_node *node)
// {
// 	t_node *cur;

// 	cur = node;
// 	while (cur)
// 	{
// 		if (cur->args == ">")
// 			redirect_out(cur->next->args);
// 		cur = cur->next;
// 	}
// }