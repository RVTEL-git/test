/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dispaly.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barmarti <barmarti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 20:36:13 by barmarti          #+#    #+#             */
/*   Updated: 2025/08/08 16:31:31 by barmarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	display_lexer_results(t_token *lst_tokens)
{
	t_token	*head;

	if (!lst_tokens)
		return ;
	head = lst_tokens;
	while (lst_tokens)
	{
		printf("VALUE: [%s]				TYPE: [%u]	TO_EXP: [%d]	TO_JOIN: [%d]\n", \
		lst_tokens->value, lst_tokens->type, \
		lst_tokens->to_exp, lst_tokens->to_join);
		lst_tokens = lst_tokens->next;
	}
	clear_tokens_lst(&head);
}

void	print_indent(int depth)
{
	for (int i = 0; i < depth; i++)
		printf("    ");
}

void	print_cmd_node(t_cmd *cmd, int depth)
{
	t_cmd	*current;

	current = cmd;
	while (cmd)
	{
		print_indent(depth);
		if (cmd->name)
			printf("Command: %s\n", cmd->name);
		else
			printf("Command: (null)\n");

		print_indent(depth);
		printf("Args: ");
		if (cmd->args)
		{
			int	i = 0;
			while (cmd->args[i])
			{
				printf("%s ", cmd->args[i]);
				i++;
			}
		}
		else
			printf("(none)");
		printf("\n");

		print_indent(depth);
		printf("FD in: %d, FD out: %d\n", cmd->fd_in, cmd->fd_out);

		cmd = cmd->next;
	}
}

void	display_ast_results(t_ast *node, int depth, char branch)
{
	if (!node)
		return;

	print_indent(depth);
	if (branch != ' ')
		printf("%c── ", branch);
	else
		printf("    ");
	if (node->node_type == NODE_PIPE)
		printf("[PIPE]\n");
	else if (node->node_type == NODE_CMD)
	{
		printf("[CMD]\n");
		print_cmd_node(node->cmds, depth + 1);
	}

	if (node->left)
		display_ast_results(node->left, depth + 1, 'L');
	if (node->right)
		display_ast_results(node->right, depth + 1, 'R');
}

void	display_env(t_env *env)
{
	t_env	*tmp;

	tmp = env;
	while (tmp)
	{
		ft_printf("%s=%s\n", tmp->key, tmp->value);
		tmp = tmp->next;
	}
}
