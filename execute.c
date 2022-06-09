/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wwan-taj <wwan-taj@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/08 15:07:40 by wwan-taj          #+#    #+#             */
/*   Updated: 2022/06/09 18:47:59 by wwan-taj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**argarr(t_shell *shell, t_cmdgroup *grp)
{
	t_token	*token;
	int		argcount;
	char	**argv;
	int		i;

	(void)shell;
	token = grp->tokens->next;
	argcount = 0;
	while (token != NULL && token->type == ARG)
	{
		argcount++;
		token = token->next;
	}
	argv = malloc(sizeof(char *) * argcount + 2);
	i = 0;
	token = grp->tokens;
	while (i < argcount + 1)
	{
		argv[i] = ft_strdup(token->str);
		token = token->next;
		i++;
	}
	argv[i] = NULL;
	return (argv);
}

char	*getcommandpath2(char **binpath, char *str, int i)
{
	char	*path;
	char	*path2;

	path2 = ft_strjoin("/", str);
	path = ft_strjoin(binpath[i - 1], path2);
	free(path2);
	return (path);
}

char	*getcommandpath(t_shell *shell, char *str, int i)
{
	char			**binpath;
	DIR				*dir;
	struct dirent	*entity;
	char			*path;

	binpath = ft_split(ft_getenv(shell->sh_env, "PATH"), ':');
	while (binpath[i] != NULL)
	{
		dir = opendir(binpath[i++]);
		entity = readdir(dir);
		while (entity != NULL)
		{
			if (ft_strcmp(str, entity->d_name) == 0)
			{	
				closedir(dir);
				path = getcommandpath2(binpath, str, i);
				free2d(binpath);
				return (path);
			}
			entity = readdir(dir);
		}
		closedir(dir);
	}
	free2d(binpath);
	return (NULL);
}

int	ft_execve(t_shell *shell)
{
	pid_t	pid;
	char	*str;
	char	**args;
	char	*path;
	int		status;

	pid = fork();
	if (pid == 0)
	{
		str = shell->cmdgroup->tokens->str;
		args = argarr(shell, shell->cmdgroup);
		path = getcommandpath(shell, shell->cmdgroup->tokens->str, 0);
		execve(str, args, shell->sh_env);
		execve(path, args, shell->sh_env);
		exit(1);
	}
	else
		waitpid(0, &status, 0);
	return (status);
}

void	exe_builtin(t_shell *shell)
{
	if (ft_strcmp(shell->cmdgroup->tokens->str, "echo") == 0)
		exe_echo(shell, shell->cmdgroup);
	else if (ft_strcmp(shell->cmdgroup->tokens->str, "cd") == 0)
		exe_cd(shell, shell->cmdgroup);
	else if (ft_strcmp(shell->cmdgroup->tokens->str, "pwd") == 0)
		exe_pwd(shell, shell->cmdgroup);
	else if (ft_strcmp(shell->cmdgroup->tokens->str, "export") == 0)
		exe_export(shell, shell->cmdgroup);
	else if (ft_strcmp(shell->cmdgroup->tokens->str, "unset") == 0)
		exe_unset(shell, shell->cmdgroup);
	else if (ft_strcmp(shell->cmdgroup->tokens->str, "env") == 0)
		showenv(shell);
	else if (ft_strcmp(shell->cmdgroup->tokens->str, "exit") == 0)
		exe_exit(shell, shell->cmdgroup);
	else if (ft_execve(shell))
		printerror(shell, "Error. Command not found.\n", NOCOMMAND);
}
