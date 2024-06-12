/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_free.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hzaz <hzaz@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 15:02:38 by vabertau          #+#    #+#             */
/*   Updated: 2024/06/12 10:58:59 by hzaz             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	free_redir(t_exec *tmp_exec)
{
	t_token	*tmp_redir;
	t_token	*to_free;

	tmp_redir = tmp_exec->redir;
	to_free = tmp_exec->redir;
	while (tmp_redir)
	{
		if (tmp_redir->word)
			free(tmp_redir->word);
		tmp_redir->word = NULL;
		tmp_redir = tmp_redir->next;
		free(to_free);
		to_free = NULL;
		to_free = tmp_redir;
	}
}

void	free_exec(t_data *data)
{
	t_exec	*tmp_exec;
	t_exec	*tmp_exec_tf;

	tmp_exec = data->exec;
	while (tmp_exec)
	{
		/*if (tmp_exec->redir)
			free(tmp_exec->redir);*/
		free_redir(tmp_exec);
		if (tmp_exec->full_cmd)
			free(tmp_exec->full_cmd);
		if (tmp_exec->split_cmd)
			free(tmp_exec->split_cmd);
		tmp_exec_tf = tmp_exec;
		tmp_exec = tmp_exec->next;
		free(tmp_exec_tf);
	}
}

// SHOULD NOT EXIT AS MINISHELL IS A LOOP GIVING BACK PROMPT

void	free_all(t_data *data)
{
	int	i;

	i = -1;
	if (data->cmdline != NULL)
		free(data->cmdline);
	if (data->token)
		free_token(data->token);
	if (data->exec)
		free_exec(data);
	if (data->is_bq)
		free(data->is_bq);
	if (data->pipe_fds)
		free(data->pipe_fds);
	if (data->char_env)
	{
		while (data->char_env[++i])
			if (data->char_env[i])
				free(data->char_env[i]);
		free(data->char_env);
	}
}

void	exit_free(t_data *data, int exit_code)
{
	free_all(data);
	free_env(data);
	exit(exit_code);
}

void	set_exitloop_free(t_data *data)
{
	data->sh_exit_loop = 1;
}
