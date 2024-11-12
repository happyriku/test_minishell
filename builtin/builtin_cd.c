#include "../include/minishell.h"

char	*get_directory_path(char *path, char **rest)
{
	char	*new_path;
	char	*start;
	int		i = 8;

	start = path;
	while (1)
	{
		while (*path && *path != '/')
			path++;
		if (*path == '\0')
			break ;
		path++;
		start = path;
	}
	new_path = (char *)malloc(sizeof(char) * (start - *rest));
	if (!new_path)
		fatal_error("malloc");
	ft_strncpy(new_path, *rest, (start - *rest));
	return (new_path);
}

void	change_parent_directory_from_current_directory(void)
{
	char	*path;
	char	buf[PATH_MAX];
	char	*str;
	char	*new_path;

	path = getcwd(buf, sizeof(buf));
	if (!path)
		fatal_error("getcwd");
	new_path = get_directory_path(path, &path);
	if (!new_path)
		fatal_error("malloc");
	if (chdir(new_path) != 0)
		fatal_error("chdir");
}

int	builtin_cd(char **args)
{
	char	*path;
	char	buf[PATH_MAX];
	//引数に何も入力されなかったとき
	//unsetでHOMEdirの環境変数を消去したときgetenvはNULLを返すのか，シェルはどのような挙動をするのが正しいのかが気になる
	if (!args[1] || strcmp(args[1], "~") == 0)
	{
		path = getenv("HOME");
		if (!path)
			fatal_error("getenv");
		if (chdir(path) != 0)
			fatal_error("chdir");
	}
	else
	{
		if (strcmp(args[1], ".") == 0)
		{
			path = getcwd(buf, sizeof(buf));
			if (!path)
				fatal_error("getcwd");
			if (chdir(path) != 0)
				fatal_error("chdir");
		}
		else if (strcmp(args[1], "..") == 0)
			change_parent_directory_from_current_directory();

	}
	return (0);
}