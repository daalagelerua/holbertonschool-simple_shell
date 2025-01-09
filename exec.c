#include "shell.h"

/**
* execute_command - crée les forks et execute les commandes
* @cmd_argv: array d'arg
* @argv: nom programme
*/

void execute_command(char **cmd_argv, char *argv)
{
pid_t pid = fork();
	if (pid == -1)
	{
	perror(argv);
		return;
	}

	if (pid == 0)
		{
		if (execvp(cmd_argv[0], cmd_argv) == -1)
			{
			perror(argv);
			}
		exit(EXIT_FAILURE);
		}
	else
		{
		wait(NULL);
		}
}
