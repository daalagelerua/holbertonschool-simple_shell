#include "shell.h"

/**
* execute_command - crée les forks et execute les commandes
* @cmd_argv: array d'arg
* @argv: vecteur d'argument de la fonction main
*/

void execute_command(char **cmd_argv, char **argv)
{
	pid_t pid;
	int status;
	char *cmd_path;

/* Vérifier si la commande est un chemin absolu/relatif valide */
	if (access(cmd_argv[0], X_OK) == 0)
	{
		cmd_path = cmd_argv[0];
	}
	else
	{
		/* Rechercher dans le PATH */
		cmd_path = find_command_path(cmd_argv[0]);
		if (cmd_path == NULL)
		{
			fprintf(stderr, "%s: 1: %s: not found\n", argv[0], cmd_argv[0]);
			return;
		}
	}

	/* Créer un processus enfant pour exécuter la commande */
	pid = fork();
	if (pid == -1)
	{
		perror("Erreur fork");
		free(cmd_path);
		return;
	}

	if (pid == 0) /* Processus enfant */
	{
	printf("%s\n%s\n", cmd_path, cmd_argv[0]);
		if (execve(cmd_path, cmd_argv, environ) == -1)
		{
			fprintf(stderr, "%s: 1: %s: not found\n", argv[0], cmd_argv[0]);
			free(cmd_path);
			exit(EXIT_FAILURE);
		}
	}
	else /* Processus parent */
	{
		wait(&status); /* Attend que l'enfant termine */
	}

	if (cmd_path != cmd_argv[0])
		free(cmd_path);
}
