/* ************************************************************************** */
/*	                                                                        */
/*                                                        :::      ::::::::   */
/*   handle_errors.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osurcouf <.@student.42lisboa.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/24 13:07:08 by osurcouf          #+#    #+#             */
/*   Updated: 2021/09/24 13:07:10 by osurcouf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/*
** 🦕
** Errors and Exit
**
** This functions print the error messages and verify that all
** memory is freed before exiting to avoid leaks !
** 🦕
*/

void	handle_error(t_error error, t_pipex *pipex)
{
	if (pipex)
		free_pipex(pipex);
	ft_putstr_fd(""LIGHT_RED"Error\n"END_COLOR"", STDERR_FILENO);
	if (error == WRONG_ARG)
	{
		ft_putstr_fd("Wrong number of arguments.\nWrite ./pipex", STDERR_FILENO);
		ft_putstr_fd(""WHITE" file1 cmd1 cmd2 file2"END_COLOR"", STDERR_FILENO);
	}
	else if (error == INIT_PATH)
		ft_putstr_fd("Failed to allocate cmd_paths.\n", STDERR_FILENO);
	else if (error == PIPE)
		ft_putstr_fd("Failed to pipe.\n", STDERR_FILENO);
	else if (error == FORK)
		ft_putstr_fd("Failed to fork.\n", STDERR_FILENO);
	else if (error == OPEN_INFILE)
		ft_putstr_fd("Failed to open infile.\n", STDERR_FILENO);
	else if (error == OPEN_OUTFILE)
		ft_putstr_fd("Failed to open outfile.\n", STDERR_FILENO);
	else if (error == CMD1_EXECUTION)
		ft_putstr_fd("Failed to execute the first command.\n", STDERR_FILENO);
	else if (error == CMD2_EXECUTION)
		ft_putstr_fd("Failed to execute the second command.\n", STDERR_FILENO);
	else if (error == CMD_ACCESS)
		ft_putstr_fd("Could not access a command.\n", STDERR_FILENO);
	exit(EXIT_FAILURE);
}

void	free_pipex(t_pipex *pipex)
{
	if (pipex->paths)
		free_split(pipex->paths);
	if (pipex->cmd_path)
		free(pipex->cmd_path);
	if (pipex->command)
		free_split(pipex->command);
}

void	free_split(char **split)
{
	int	i;

	i = 0;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
}
