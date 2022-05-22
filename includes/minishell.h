#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdlib.h>
# include "libft.h"

# define COMMAND 1 // eg: echo, pwd, grep, awk, etc
# define ARG 2 // inputs after commands. eg: "hello world"
# define INPUT 3 // redirection to left <. Grab input from fd on the right and pass as argument to left
# define OUTPUT 4 // redirection to right >. Redirect output from left to right which should be an fd
# define APPEND 5 // double redirection >>. Take output from left and append to fd on the right
# define RDINPUT 6 // double redirection <<. Read input from user until EOF is met as pass as argument to programme on left
# define PIPE 7 // Take output from left of pipe and pass as argument to command on right of pipe
# define LASTARG 8 //

// typedef struct s_cmdgrp
// {
// 	char	*cmd;
// 	char	*arg;
// 	char	*flag;
// }	t_cmdgrp;

typedef struct s_token
{
	char			*str;
	int				type;
	struct s_token	*next;
	struct s_token	*prev;
}	t_token;

typedef struct s_cmdgroup
{
	t_token				*tokens;
	int					cmdcount;
	// int				tkn_count;
	struct	s_cmdgroup  *next;
}	t_cmdgroup;

int			collecttoken(char *line, t_cmdgroup *cmd);
void		printerror(t_cmdgroup *cmd);
void		filllst(t_cmdgroup *cmd, char *str, int i, int len);
int			addlist(t_cmdgroup *cmd, char *str, int i, int len); // Maybe to replace filllst
void		addnewlst(t_token *lst);
void		clearmemory(t_cmdgroup *lst);
void		showlist(t_cmdgroup *cmd);
t_cmdgroup	*creategroup(int count);
t_cmdgroup	*collectcmdgroup(char *line);

#endif