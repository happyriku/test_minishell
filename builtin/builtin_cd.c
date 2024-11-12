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

void	change_root_directory(void)
{
	if (chdir("/") != 0)
		fatal_error("chdir");
}

int	exec_change_directory(char *path)
{
	char	*new_path;

	if (strncmp(path, "~", 1) == 0)
	{
		if (strcmp(path, "~/") == 0)
			return (change_root_directory(), 0);
		new_path = getenv("HOME");
		if (!path)
			fatal_error("getenv");
		if (chdir(new_path) != 0)
			return (1);
		path += 2;
		if (chdir(path) != 0)
			return (1);
	}
	else
	{
		if (chdir(path) != 0)
			return (1);
	}
	return (0);
}

int	cd_error(char *str)
{
	if (strncmp(str, "~/", 2) == 0)
	{
		str += 2;
		printf("-bash: cd: /home/rishibas/%s: No such file or directory\n", str);
	}
	else
		printf("-bash: cd: %s: No such file or directory\n", str);
	return (0);
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
			return (cd_error(path));
	}
	else
	{
		if (strcmp(args[1], ".") == 0)
		{
			path = getcwd(buf, sizeof(buf));
			if (!path)
				fatal_error("getcwd");
			if (chdir(path) != 0)
				return (cd_error(path));
		}
		else if (strcmp(args[1], "..") == 0)
			change_parent_directory_from_current_directory();
		else if (strcmp(args[1], "/") == 0)
			change_root_directory();
		else
		{
			if (exec_change_directory(args[1]) != 0)
				return (cd_error(args[1]));
		}
	}
	return (0);
}