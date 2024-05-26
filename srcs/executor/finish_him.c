/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   finish_him.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hzaz <hzaz@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/08 21:08:17 by hedi              #+#    #+#             */
/*   Updated: 2024/05/26 19:05:00 by hzaz             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	exec_build(t_data *shell, char **f)
{
	if (ft_same_str(f[0], "echo", 4))
		shell->last_return_code = ft_echo(f);
	else if (ft_same_str(f[0], "cd", 2))
		shell->last_return_code = ft_cd(f, shell);
	else if (ft_same_str(f[0], "pwd", 3))
		shell->last_return_code = ft_pwd(shell);
	else if (ft_same_str(f[0], "export", 6))
		shell->last_return_code = ft_export(f, shell);
	else if (ft_same_str(f[0], "unset", 5))
		shell->last_return_code = ft_unset(shell, f);
	else if (ft_same_str(f[0], "env", 3))
		shell->last_return_code = ft_env(shell, f);
	else if (ft_same_str(f[0], "exit", 4))
		ft_exit(f, shell);
	else
		return ;
	exit_free(shell, 0);
}
// Aucun built-in correspondant trouvé
// if (ft_same_str(f[0], "echo", 4))
// 	ft_echo(f);
// else if (ft_same_str(f[0], "cd", 2))
// 	ft_cd();
// printf("test");

static void	init_exec_path(int *i, t_env **e, t_data *sh)
{
	*i = -1;
	*e = sh->env;
}

static void	incr_j(t_env *e, int *j)
{
	while (e->val[*j] != ':' && e->val[*j] && e->val)
		(*j)++;
}

void	exec_path(t_data *sh, char **f, char *tmp)
{
	int		i;
	int		j;
	int		k;
	char	*ret;
	t_env	*e;

	init_exec_path(&i, &e, sh);
	while (e)
	{
		if (ft_same_str(e->var_name, "PATH", 4))
		{
			j = -1;
			while (e->val && e->val[++j])
			{
				k = j;
				incr_j(e, &j);
				ret = join_free1(ft_substr(e->val, k, ((j) - k)), tmp);
				if (e->val[j] == ':')
					if (access(ret, F_OK) == 0)
						execve(ret, f, sh->char_env);
				free(ret);
			}
		}
		e = e->next;
	}
}

int is_directory(const char *path) {
    struct stat fileStat;
    if(stat(path, &fileStat) < 0) {
        perror("stat");
        return 0;
    }

    return S_ISDIR(fileStat.st_mode);
}

void	exec_cmd(t_data *shell, t_exec *cmd)
{
	char	*ret;
	char	*tmp;
	char	**f;

	handle_redirections(cmd, shell);
	f = cmd->split_cmd;
	if (f[0][0] == '.' && f[0][1]=='/')
		execve(f[0], f, shell->char_env);
	exec_build(shell, f);
	tmp = ft_strjoin("/", f[0]);
	if (access(tmp, F_OK) == 0)
		execve(tmp, f, shell->envp);
	else
		exec_path(shell, f, tmp);
	if (is_directory(f[0])
		ret = ft_strdup("is a directory.\n");
	else
		ret = join_free1(ft_strjoin("command not found: ", f[0]), "\n");
	ft_putstr_fd_free(ret, 2);
	free(tmp);
	exit_free(shell, 127);
}
