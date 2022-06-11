/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wwan-taj <wwan-taj@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/19 18:21:35 by wwan-taj          #+#    #+#             */
/*   Updated: 2022/06/11 20:47:51 by wwan-taj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*createlist(char *str, int i, int len)
{
	t_token	*newlist;

	if (!str || i < 0 || len < 0)
		return (NULL);
	newlist = malloc(sizeof(t_token));
	newlist->str = ft_substr(str, i, len);
	newlist->next = NULL;
	newlist->prev = NULL;
	return (newlist);
}

void	creategroup(t_cmdgroup **cmdgroup, int count)
{
	t_cmdgroup	*lst;
	t_cmdgroup	*new;

	*cmdgroup = malloc(sizeof(t_cmdgroup));
	(*cmdgroup)->tokens = NULL;
	(*cmdgroup)->cmdcnt = 0;
	(*cmdgroup)->next = NULL;
	lst = *cmdgroup;
	while (count-- > 1)
	{
		new = malloc(sizeof(t_cmdgroup));
		new->tokens = NULL;
		lst->next = new;
		lst->next->next = NULL;
		lst = lst->next;
	}
}

int	addlist(t_cmdgroup *cmd, char *str, int i, int len)
{
	t_token	*lst;
	t_token	*temp;
	t_token	*newlist;

	newlist = createlist(str, i, len);
	if (!newlist)
		return (EXIT_FAILURE);
	lst = cmd->tokens;
	if (lst == NULL)
	{
		cmd->tokens = newlist;
		return (EXIT_SUCCESS);
	}
	while (lst->next != NULL)
	{
		temp = lst;
		lst = lst->next;
		lst->prev = temp;
	}
	newlist->prev = lst;
	lst->next = newlist;
	return (EXIT_SUCCESS);
}

void	showlist(t_cmdgroup *cmd)
{
	t_token		*firsttoken;
	t_cmdgroup	*firstcmd;
	int			i;

	firstcmd = cmd;
	i = 0;
	while (cmd != NULL)
	{
		printf("%d\n", i);
		firsttoken = cmd->tokens;
		while (cmd->tokens != NULL)
		{
			printf("str: (%s)\t type: %d\n", cmd->tokens->str, cmd->tokens->type);
			cmd->tokens = cmd->tokens->next;
		}
		cmd->tokens = firsttoken;
		cmd = cmd->next;
		i++;
	}
	cmd = firstcmd;
}

int	countcmdgroups(char *line)
{
	int		count;
	int		openquote;
	char	quotetype;
	int		i;

	i = 0;
	openquote = 0;
	count = 1;
	while (line[i] != '\0')
	{
		if ((line[i] == '\'' || line[i] == '"') && openquote == 0)
		{
			openquote = 1;
			quotetype = line[i];
		}
		else if (line[i] == quotetype)
			openquote = 0;
		if (line[i] == '|' && openquote == 0)
			count++;
		i++;
	}
	return (count);
}
