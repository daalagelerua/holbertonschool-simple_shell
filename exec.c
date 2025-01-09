#include "shell.h"

/**
* execute_command - crée les forks et execute les commandes
* @cmd_argv: array d'arg
* @argv: nom programme
*/

void execute_command(char **cmd_argv, char *argv)
{
char *cmd_path;

pid_t pid = fork();
	if (pid == -1)
	{
	perror(argv);
		return;
	}

	if (pid == 0)
		{
		cmd_path = find_command_path(argv);
		if (cmd_path == NULL)
			{
			perror("command not found");
			exit(EXIT_FAILURE);
			}
		if (execve(cmd_path, cmd_argv, environ) == -1)
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
