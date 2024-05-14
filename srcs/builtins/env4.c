/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env4.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vabertau <vabertau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 11:34:46 by vabertau          #+#    #+#             */
/*   Updated: 2024/05/14 11:37:13 by vabertau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	var_in_env(char *s, t_data *shell)
{
	int		ret;
	int		j;
	t_env	*tmp;
	char	**var;

	var = split_var(s, shell);
	tmp = shell->env;
	if (!var)
	{
		perror("malloc");
		exit_free(shell, EXIT_FAILURE);
	}
	ret = -1;
	j = -1;
	while (tmp)
	{
		if (!ft_strcmp(tmp->var_name, var[0]))
			ret = tmp->index;
		tmp = tmp->next;
	}
	if (var[1])
		free(var[1]);
	free(var[0]);
	free(var);
	return (ret);
}

void	ft_add_env(char *s, t_data *shell)
{
	char	**str;
	t_env	*new_node;
	t_env	*e;

	str = split_var(s, shell);
	if (!str)
		return ;
	new_node = malloc(sizeof(t_env));
	if (!new_node)
	{
		perror("malloc");
		return ;
	}
	if (!str[1])
	{
		new_node->var = ft_strdup(str[0]);
		new_node->var_name = new_node->val = NULL;
		new_node->next = NULL;
	}
	else
	{
		new_node->var = join_free1(ft_strjoin(str[0], "="), str[1]);
		new_node->var_name = ft_strdup(str[0]);
		new_node->val = str[1] ? ft_strdup(str[1]) : NULL;
		new_node->next = NULL;
	}
	if (shell->env == NULL)
	{
		shell->env = new_node;
		new_node->index = 0;
	}
	else
	{
		e = shell->env;
		while (e->next != NULL)
			e = e->next;
		e->next = new_node;
		new_node->index = e->index + 1;
	}
}

int	ft_putenv(char *s, t_data *shell)
{
	int	pos;

	pos = var_in_env(s, shell);
	if (pos > -1)
		ft_update_env(s, shell, pos);
	else
		ft_add_env(s, shell);
	return (1);
}
