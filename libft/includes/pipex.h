/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osurcouf <.@student.42lisboa.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/24 12:11:46 by osurcouf          #+#    #+#             */
/*   Updated: 2021/09/24 12:11:47 by osurcouf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <unistd.h>
# include <string.h>
# include <stdlib.h>
# include <stdio.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <errno.h>
# include <limits.h>
# include <stdbool.h>
# include "../libft/libft.h"
# include "colors.h"

/* ************************************************************************** */
/*                                  Pipex                                     */
/* ************************************************************************** */

typedef struct s_pipex
{
	int	 	infile_fd;
	char	*infile;
	int	 	outfile_fd;
	char	*outfile;
	char	**paths;
	char	*cmd_path;
	char	**command;
	char	**argv;
	char	**envp;
}				t_pipex;

void	run_pipex(t_pipex *pipex);
void	child_process(t_pipex *pipex, int *end);
void	parent_process(t_pipex *pipex, int *end);

/* ************************************************************************** */
/*								  Utils		       			                  */
/* ************************************************************************** */

t_pipex	init_pipex_values(int argc, char **argv, char **envp);
char	**get_cmd_paths(char **envp);
char	**finish_paths_by_slash(char **paths);
bool	valid_command(t_pipex *pipex, int count_arg);

/* ************************************************************************** */
/*								  Errors									*/
/* ************************************************************************** */

typedef enum e_memory_allocation
{
	DONT_FREE_POINTER,
	FREE_POINTER,
}			t_memory_allocation;

typedef enum e_error
{
	ERROR = -1,
	EXISTS = 0,
	CHILD_PROCESS = 0,
	PARENT_PROCESS,
	WRONG_ARG,
	MEMORY,
	INIT_PATH,
	PIPE,
	FORK,
	OPEN_INFILE,
	OPEN_OUTFILE,
	CMD1_EXECUTION,
	CMD2_EXECUTION,
	CMD_ACCESS,
}			t_error;

void	handle_error(t_error error, t_pipex *pipex);
void	free_pipex(t_pipex *pipex);
void	free_split(char **split);

#endif