/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env3.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vabertau <vabertau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 11:32:12 by vabertau          #+#    #+#             */
/*   Updated: 2024/05/14 11:32:47 by vabertau         ###   ########.fr       */
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

int	ft_unset(char **split_cmd)
{
	int	i;

	i = 1;
	while (split_cmd[i] != NULL)
	{
		unsetenv(split_cmd[i]);
		i++;
	}
	return (0);
}

int	ft_env(char **envp)
{
	int	i;

	i = 0;
	while (envp[i] != NULL)
	{
		ft_putstr_fd(envp[i], STDOUT_FILENO);
		ft_putstr_fd("\n", STDOUT_FILENO);
		i++;
	}
	return (0);
}
