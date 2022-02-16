/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osurcouf <.@student.42lisboa.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/01 22:54:35 by osurcouf          #+#    #+#             */
/*   Updated: 2021/10/01 22:54:37 by osurcouf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/*
** 🦕
** Utilities
**
** Those functions complement the main pipex functions.
** They initialize memory and retrive the commands paths
** and elements.
** 🦕
*/

/*
** Function : init_pipex_values
** This function initialise all the variables of the
** pipex struct. 
*/

t_pipex	init_pipex_values(int argc, char **argv, char **envp)
{
	t_pipex	pipex;

	pipex.infile = argv[1];
	pipex.outfile = argv[argc - 1];
	pipex.paths = get_cmd_paths(envp);
	if (!pipex.paths)
		handle_error(INIT_PATH, &pipex);
	pipex.command = NULL;
	pipex.cmd_path = NULL;
	pipex.argv = argv;
	pipex.envp = envp;
	return (pipex);
}

/*
** Function : get_cmd_paths
** This function get all the different paths to functions from
** the environement variable (envp).
**
** The envp looks like this :
**					NAME=localname
**					HOST=localhost
**					PATH=all_the_paths:path:path:path
**					WHATEVER=whatever
**
** We only want the paths and we want all of them seperately so we
** use ft_split with ':' as the separator and an offset of 5 to start
** after 'PATH='. Then, because they'll need a '/' before we add the 
** command name to those paths, we add it already.
*/

char	**get_cmd_paths(char **envp)
{
	char	**raw_paths;
	char	**paths;

	while (*envp && ft_strncmp(*envp, "PATH=", 5))
		envp++;
	raw_paths = ft_split((*envp + 5), ':');
	paths = finish_paths_by_slash(raw_paths);
	free_split(raw_paths);
	return (paths);
}

/*
** Function : finish_paths_by_slash
** Very self explanatory, this function adds a '/' at the
** end of every single paths retrieved in the previous function.
*/

char	**finish_paths_by_slash(char **raw_paths)
{
	int		nb;
	char	**correct_paths;

	nb = 0;
	while (raw_paths[nb])
		nb++;
	correct_paths = (char **)malloc(sizeof(char *) * (nb + 1));
	if (!correct_paths)
		return (NULL);
	nb = 0;
	while (raw_paths[nb])
	{
		correct_paths[nb] = ft_strjoin(raw_paths[nb], "/");
		nb++;
	}
	correct_paths[nb] = 0;
	return (correct_paths);
}

/*
** Function : valid_command
** This function checks that the command fed to execve exists and
** is executable. 
** First we retrieve the command with ft_split so we also get all the flags.
** Then we use the access function which checks with F_OK if the command exists
** and X_OK if it's executable.
*/

bool	valid_command(t_pipex *pipex, int count_arg)
{
	int	i;
	int	offset;

	i = 0;
	offset = 2;
	pipex->command = ft_split(pipex->argv[count_arg + offset], ' ');
	while (pipex->paths[i])
	{
		if (pipex->cmd_path)
			free(pipex->cmd_path);
		pipex->cmd_path = ft_strjoin(pipex->paths[i], pipex->command[0]);
		if (access(pipex->cmd_path, F_OK) == EXISTS
			&& access(pipex->cmd_path, X_OK) == EXISTS)
			return (true);
		i++;
	}
	return (false);
}
