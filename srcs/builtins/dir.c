/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dir.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hzaz <hzaz@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 00:46:16 by hedi              #+#    #+#             */
/*   Updated: 2024/06/12 13:48:43 by hzaz             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
t_env	*get_var(char *name, t_data *shell)
{
	int	i;
	t_env	*tmp;
	int	cpt;

	i = var_in_env(name, shell);
	tmp = shell->env;
	cpt = -1;
	if (i < 0)
		return (NULL);
	while (++cpt < i && tmp)
		tmp = tmp->next;
	if (tmp)
		return (tmp);
	return (NULL);
}

int	ft_cd(char **split_cmd, t_data *shell)
{
	int	ret;
	char	*tmp;
	char	*tmp2;

	ret = 0;
	if (split_cmd[1] && split_cmd[2])
	{
		ft_putstr_fd("cd: too many arguments\n", 2);
		return (1);
	}
	tmp = ft_strjoin("OLDPWD=", get_var("PWD",shell)->val);
	if (!split_cmd[1])
	{
		if (!var_in_env("HOME", shell))
		{
			perror("cd: HOME not set");
			//free(tmp);
			return (1);
		}
		ret = chdir(get_var("HOME",shell)->val);
	}
	else
		ret = chdir(split_cmd[1]);
	if (ret != 0)
	{
		perror("cd");
		free(tmp);
		return (1);
	}
	tmp2 = getcwd(NULL, 0);
	if (!tmp2)
		perror("cd: error retrieving current directory: getcwd: cannot access parent directories: No such file or directory");
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
// int	ft_cd(char **split_cmd, t_data *shell)
// {
// 	int	ret;
// 	char	*tmp;
// 	char	*tmp2;

// 	ret = 0;
// 	if (split_cmd[2])
// 	{
// 		ft_putstr_fd("cd: too many arguments", 2);
// 		return (1);
// 	}
// 	tmp = join_free2("OLDPWD=", getcwd(NULL, 0));
// 	ret = chdir(split_cmd[1]);
// 	if (split_cmd[1] == NULL || ret != 0)
// 	{
// 		perror("cd");
// 		//free_all(shell); //enleve car genere un double free et inutile car est free ensuite
// 	}
// 	tmp2 = join_free2("PWD=", getcwd(NULL, 0));
// 	if (var_in_env(tmp,shell) > -1)
// 		ft_update_env(tmp, shell, var_in_env(tmp,shell));
// 	else
// 		ft_add_env(tmp, shell);
// 	if (var_in_env(tmp2,shell) > -1)
// 		ft_update_env(tmp2, shell, var_in_env(tmp2,shell));
// 	else
// 		ft_add_env(tmp2, shell);
// 	free(tmp);
// 	free(tmp2);
// 	return (ret);
// }

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
