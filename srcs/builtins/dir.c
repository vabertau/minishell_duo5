/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dir.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vabertau <vabertau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 00:46:16 by hedi              #+#    #+#             */
/*   Updated: 2024/05/29 15:27:07 by vabertau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_cd(char **split_cmd, t_data *shell)
{
	int	ret;
	char	*tmp;
	char	*tmp2;

	ret = 0;
	if (split_cmd[2])
	{
		ft_putstr_fd("cd: too many arguments", 2);
		return (1);
	}
	tmp = join_free2("OLDPWD=", getcwd(NULL, 0));
	ret = chdir(split_cmd[1]);
	if (split_cmd[1] == NULL || ret != 0)
	{
		perror("cd");
		//free_all(shell); //enleve car genere un double free et inutile car est free ensuite
	}
	tmp2 = join_free2("PWD=", getcwd(NULL, 0));
	if (var_in_env(tmp,shell) > -1)
		ft_update_env(tmp, shell, var_in_env(tmp,shell));
	else
		ft_add_env(tmp, shell);
	if (var_in_env(tmp2,shell) > -1)
		ft_update_env(tmp2, shell, var_in_env(tmp2,shell));
	else
		ft_add_env(tmp2, shell);
	free(tmp);
	free(tmp2);
	return (ret);
}

int	ft_pwd(t_data *shell)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (cwd != NULL)
	{
		ft_putstr_fd(cwd, STDOUT_FILENO);
		ft_putstr_fd("\n", STDOUT_FILENO);
		free(cwd);
		return (0);
	}
	perror("pwd");
	exit_free(shell, EXIT_FAILURE);
	return (1);
}
