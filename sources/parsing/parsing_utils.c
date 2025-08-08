/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: barmarti <barmarti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 17:01:17 by barmarti          #+#    #+#             */
/*   Updated: 2025/08/07 18:07:52 by barmarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"
#include "../../includes/minishell.h"

void	free_args(char **result, int i)
{
	while (i >= 0)
	{
		if (result[i])
			free(result[i]);
		i--;
	}
	if (result[i])
		free(result[i]);
}

bool	is_pipe(t_token *lst_token)
{
	if (lst_token->type == TOKEN_PIPE)
		return (true);
	return (false);
}

t_token	*find_pipe(t_token *lst_token)
{
	t_token	*current;

	current = lst_token;
	while (current && current->type != TOKEN_PIPE)
		current = current->next;
	if (current && current->type == TOKEN_PIPE)
		return (current);
	else
		return (NULL);
}