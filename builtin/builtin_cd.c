#include "../include/minishell.h"

int	builtin_cd(char **args)
{
	char	*path;

	//引数に何も入力されなかったとき
	//unsetでHOMEdirの環境変数を消去したときgetenvはNULLを返すのか，シェルはどのような挙動をするのが正しいのかが気になる
	if (!args[1])
	{
		path = getenv("HOME");
		if (!path)
			fatal_error("getenv");
		if (chdir(path) != 0)
			fatal_error("chdir");
	}
	else
	{
		
	}
	return (0);
}