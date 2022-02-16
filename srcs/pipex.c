/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osurcouf <.@student.42lisboa.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/24 12:11:21 by osurcouf          #+#    #+#             */
/*   Updated: 2021/09/24 12:11:23 by osurcouf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/*
** 🦕
** Pipex
**
** Pipex is a program that mimics
**		< infile cmd1 | cmd2 > outfile
** It takes the following arguments
**		./pipex infile "cmd1" "cmd2" outfile
** The logic consists with creating a pipe and rewiring the standard inputs and
** outputs so two processes can run in parallel of each other.
**
** Very good tutorial there :
**		https://csnotes.medium.com/pipex-tutorial-42-project-4469f5dd5901
** 🦕
*/

/* *********************************************************************** */
/*     each cmd needs a stdin (input) and returns an output (to stdout)    */
/*  																       */
/*      infile                                             outfile         */
/*   as stdin for cmd1                                 as stdout for cmd2  */
/*         |                        PIPE                        ↑          */
/*         |           |---------------------------|            |          */
/*         ↓             |                       |              |          */
/*        cmd1   -->    end[1]       ↔       end[0]   -->     cmd2         */
/*                       |                       |                         */
/*              cmd1   |---------------------------|  end[0]               */
/*             output                             reads end[1]             */
/*           is written                          and sends cmd1            */
/*            to end[1]                          output to cmd2            */
/*         (end[1] becomes                      (end[0] becomes            */
/*          cmd1 stdout)                           cmd2 stdin)             */
/* *********************************************************************** */

int	main(int argc, char **argv, char **envp)
{
	t_pipex	pipex;

	if (argc != 5)
		handle_error(WRONG_ARG, NULL);
	pipex = init_pipex_values(argc, argv, envp);
	run_pipex(&pipex);
	free_pipex(&pipex);
	return (EXIT_SUCCESS);
}

/*
** Run_pipex
** This function creates a pipe with two ends (fd) and forks
** the main process it's given into a parent process and a
** child process (process_id returns 0).
** Since those process are run in parallel, you want to have
** the parent wait for the child_process to be done first (wait).
**
** ATTENTION ! You want to open the outfile already because it needs to be
** created first and will be created whatever happens. If you don't create it
** at the beginning, a command like ls -l | wc -l will be off count because
** ls -l will count the number of files when outfile has not been created yet.
**
** A good ressource on all those functions is this series of classes
** on Code Vault :
** 	https://code-vault.net/course/
**			46qpfr4tkz:1603732431896/lesson/16non3fdoh:1603732431950
*/

void	run_pipex(t_pipex *pipex)
{
	int		end[2];
	pid_t	process_id;
	int		infile_fd;
	int		outfile_fd;

	if (pipe(end) == ERROR)
		return (handle_error(PIPE, pipex));
	process_id = fork();
	if (process_id == ERROR)
		return (handle_error(FORK, pipex));
	infile_fd = open(pipex->infile, O_RDONLY);
	if (infile_fd == ERROR)
		handle_error(OPEN_INFILE, pipex);
	outfile_fd = open(pipex->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (outfile_fd == ERROR)
		handle_error(OPEN_OUTFILE, pipex);
	if (process_id == CHILD_PROCESS)
		child_process(pipex, end, infile_fd);
	else
	{
		waitpid(process_id, NULL, 0);
		parent_process(pipex, end, outfile_fd);
	}
}

/*
** Fuction : child_process
** It will first open the infile (0_RDONLY) before duplicating it with
** dup2. Dup2 will then swap STDIN (0) so that the standard input will
** be the infile_fd and the command will read from there when it's executed.
** Then we want the exit of the pipe to be the standard output so that when the
** command runs, it's result are forwarded there.
** Then we check if the command we were given in the program is valid and we
** run it. If it runs, everything given to execve as an argument and everything
** happening after execve has been deleted.
**
** Important : execve gets stuck if all the fds are not closed. So close all
** the fds ! 
*/

void	child_process(t_pipex *pipex, int *end, int infile_fd)
{
	close(end[0]);
	dup2(infile_fd, STDIN_FILENO);
	close(infile_fd);
	dup2(end[1], STDOUT_FILENO);
	close(end[1]);
	if (valid_command(pipex, 0))
		execve(pipex->cmd_path, pipex->command, pipex->envp);
	else
		handle_error(CMD1_EXECUTION, pipex);
}

/*
** Fuction : Parent_process
** The parent process will take over once the child process is done.
** Its behaviour is similar but with key difference.
**
** 	The outfile
** First, the outfile has to be created if it doesn't exists (O_CREAT). 
** It will also need to be readable and writable (O_RDWR).
** If it already exists, it needs to be overwritten (O_TRUNC gets
** everything back to 0).
** Finally, if it's created, its permissions need to be 0644.
**
** Then we need to copy and swap the outfile fd so it's the standard
** output && tranform the beginning of the pipe as the standard input.
** We check that the command is a valid one and execute it.
**
** Same warnings as for the child process : close fds and after execve
** nothing happens anymore ! 
*/

void	parent_process(t_pipex *pipex, int *end, int outfile_fd)
{
	close(end[1]);
	dup2(outfile_fd, STDOUT_FILENO);
	close(outfile_fd);
	dup2(end[0], STDIN_FILENO);
	close(end[0]);
	if (valid_command(pipex, 1))
		execve(pipex->cmd_path, pipex->command, pipex->envp);
	else
		handle_error(CMD1_EXECUTION, pipex);
}
