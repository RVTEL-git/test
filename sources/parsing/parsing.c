/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barmarti <barmarti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 12:07:21 by cscache           #+#    #+#             */
/*   Updated: 2025/08/06 18:52:07 by barmarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"
#include "../../includes/minishell.h"

/* static void	add_to_lst_cmd(t_cmd **lst, t_cmd *new)
{
	t_cmd	*last;

	last = NULL;
	if (!lst || !new)
		return ;
	if (*lst)
	{
		last = *lst;
		while (last->next)
			last = last->next;
		new->prev = last;
		last->next = new;
	}
	else
		*lst = new;
} */

void	create_args_lst(char *name, t_cmd *cmd)
{
	t_list	*new_arg;

	new_arg = malloc(sizeof(t_list));
	if (!new_arg)
		return ;
	new_arg->content = ft_strdup(name);
	if (!new_arg->content)
		return ;
	new_arg->next = NULL;
	ft_lstadd_back(&cmd->tmp_args, new_arg);
}

char	**convert_list_args_to_array(t_cmd *cmd)
{
	char	**args;
	t_list	*current;
	int		i;

	args = (char **)malloc(sizeof(char *) * (ft_lstsize(cmd->tmp_args) + 1));
	if (!args)
	{
		ft_lstclear(&cmd->tmp_args, free);
		return (NULL);
	}
	i = 0;
	current = cmd->tmp_args;
	while (current)
	{
		args[i++] = ft_strdup((char *)current->content);
		if (!args[i - 1])
		{
			free_args(args, (i - 1));
			return (NULL);
		}
		current = current->next;
	}
	args[i] = NULL;
	ft_lstclear(&cmd->tmp_args, free);
	return (args);
}

// static void	add_node_to_lst_cmds(t_cmd **lst, t_cmd *new)
// {
// 	t_cmd	*last;

// 	last = NULL;
// 	if (!lst || !new)
// 		return ;
// 	if (*lst)
// 	{
// 		last = *lst;
// 		while (last->next)
// 			last = last->next;
// 		new->prev = last;
// 		last->next = new;
// 	}
// 	else
// 		*lst = new;
// 	new->next = NULL;
// }

static t_cmd	*parse_cmd_name(t_shell *shell, char *cmd_name)
{
	t_cmd	*new;

	(void)shell;
	new = (t_cmd *)malloc(sizeof(t_cmd));
	new->tmp_args = malloc(sizeof(t_list *));
	if (!new)
		return (NULL);
	new->name = ft_strdup(cmd_name);
	if (!new->name)
	{
		free(new);
		return (NULL);
	}
	return (new);
}

static t_ast	*create_pipe_node(t_ast *left, t_ast *right)
{
	t_ast	*new;

	new = malloc(sizeof (t_ast));
	if (!new)
		return (NULL);
	new->node_type = NODE_PIPE;
	new->cmds = NULL;
	new->left = left;
	new->right = right;
	return (new);
}

t_ast	*create_ast_node(t_shell *shell, t_token **lst_tokens)
{
	t_token	*current;
	t_ast	*new;
	bool	first;

	first = true;
	new = malloc(sizeof(t_ast));
	current = *lst_tokens;
	while (current && current->type != TOKEN_PIPE)
	{
		if (first)
		{
			new->cmds = parse_cmd_name(shell, current->value);
			first = false;
		}
		else
			create_args_lst(current->value, new->cmds);
		current = current->next;
	}
	if (new->cmds->tmp_args)
		new->cmds->args = convert_list_args_to_array(new->cmds);
	return (new);
}

t_ast	*set_ast(t_shell *shell, t_token *lst_tokens)
{
	t_token	*current;
	t_ast	*left;
	t_ast	*new_pipe;
	t_ast	*right;


	left = create_ast_node(shell, &lst_tokens);
	current = find_pipe(lst_tokens);
	while (current && is_pipe(current))
	{
		current = current->next;
		right = create_ast_node(shell, &lst_tokens);
		new_pipe = create_pipe_node(left, right);
		left = new_pipe;
	}
	shell->ast = *left;
	return (left);
}
