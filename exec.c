#include "shell.h"

/**
* execute_command - crée les forks et execute les commandes
* @cmd_argv: array d'arg
* @command_path: chemin de la commande
*/

void execute_command(char **cmd_argv, char *command_path)
{
pid_t pid = fork();
int status;

	if (pid == -1)
	{
	perror(command_path);
		return;
	}
	else if (pid == 0)
	{
		if (execve(command_path, cmd_argv, environ) == -1)
		{
			perror(command_path);
			exit(EXIT_FAILURE);
		}
	}
	else
		{
		wait(&status);
		}
}
