/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env3.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hzaz <hzaz@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 11:32:12 by vabertau          #+#    #+#             */
/*   Updated: 2024/05/14 12:43:18 by hzaz             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_printf_var_env(char *var)
{
	write(STDOUT_FILENO, "declare -x ", 11);
	write(STDOUT_FILENO, var, ft_strlen(var));
	write(STDOUT_FILENO, "\n", 1);
}

int	check_cmd(char *s)
{
	int	i;

	i = 0;
	if (!s[0] || (s[0] != '_' && !ft_isalpha(s[0])))
		return (0);
	while (++i && s && s[i] && s[i] != '=' && s[i] != '+')
		if ((s[i] != '_' && !ft_isalnum(s[i])))
			return (0);
	if (s[i] == '+' && s[i + 1] != '=')
		return (0);
	return (1);
}

int	ft_export(char **split_cmd, t_data *shell)
{
	int		i;
	char	*tmp;
	int		ret;
	t_env	*e;

	ret = 0;
	e = shell->env;
	if (split_cmd[1] == NULL)
	{
		i = -1;
		while (e)
		{
			ft_printf_var_env(e->var);
			e = e->next;
		}
	}
	i = 0;
	while (split_cmd[++i] != NULL)
	{
		if (!check_cmd(split_cmd[i]))
		{
			tmp = ft_strjoin("bash: export: `", split_cmd[i]);
			ft_putstr_fd_free(join_free1(tmp, "': not a valid identifier\n"),
				2);
			ret = 1;
		}
		else if (ft_putenv(split_cmd[i], shell) != 1)
		{
			perror("export");
			exit_free(shell, EXIT_FAILURE);
		}
	}
	return (ret);
}

int	check_s(char *s)
{
	int	i;

	i = -1;
	while (++i && s && s[i])
		if (s[i] != '_' && !ft_isalpha(s[i]))
			return (0);
	return (1);
}
void	free_single_env(t_env *e)
{
	if (!e)
		return ;
	if (e->val)
		free(e->val);
	if (e->var)
		free(e->var);
	if (e->var_name)
		free(e->var_name);
	free(e);
	e = NULL;
}

void	decrem_env(t_env *env)
{
	t_env	*tmp;

	tmp = env;
	while (tmp)
	{
		tmp->index--;
		tmp = tmp->next;
	}
}

int	ft_unset(t_data *shell, char **split_cmd)
{
	int		i;
	t_env	*prev;
	t_env	*current;
	int		ret;

	i = 1;
	ret = 0;
	while (split_cmd[i])
	{
		if (!check_s(split_cmd[i]))
		{
			ft_putstr_fd("bash: unset: `", 2);
			ft_putstr_fd(split_cmd[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			ret = 1;
			continue ;
		}
		current = shell->env;
		prev = NULL;
		while (current && ft_strcmp(current->var_name, split_cmd[i]) != 0)
		{
			prev = current;
			current = current->next;
		}
		if (current != NULL)
		{
			if (prev == NULL)
				shell->env = current->next;
			else
				prev->next = current->next;
			free_single_env(current);
			decrem_env(prev ? prev->next : shell->env);
		}
		i++;
	}
	return (ret);
}

int	ft_env(t_data *shell, char **f)
{
	t_env *tmp;

	tmp = shell->env;
	if (f[1])
		return ((void)ft_putstr_fd("No such file or directory\n", 2), 127);
	while (tmp)
	{
		ft_putstr_fd(tmp->var, STDOUT_FILENO);
		ft_putstr_fd("\n", STDOUT_FILENO);
		tmp = tmp->next;
	}
	return (0);
}