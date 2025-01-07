#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "shell.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "shell.h"

extern char **environ;

void execute_command(char *line, char **argv);

int main(int argc, char **argv)
{
    char *line = NULL; /* Pointeur pour stocker l'entrée utilisateur */
    size_t len = 0; /* Taille de la mémoire allouée à line */
    ssize_t nread; /* Nombre de caractères lus */
    int is_interactive = isatty(STDIN_FILENO); /* Vérifie si entrée interactive */

    (void)argc; /* argc n'est pas utilisé donc on le mute */

    while (1) /* Boucle infinie pour garder le shell actif */
    {
        if (is_interactive)
            printf("($) "); /* Affiche le prompt seulement en mode interactif */
        nread = getline(&line, &len, stdin); /* Lit l'entrée utilisateur */

        if (nread == -1) /* Vérifie si EOF ou erreur */
        {
            if (is_interactive)
                printf("\n"); /* Affiche une nouvelle ligne avant de quitter */
            break;
        }

        line[nread - 1] = '\0'; /* Supprime le '\n' en fin de commande */

token = strtok(line, " ");
while (token != NULL)
		{
		cmd_argv[i++] = token;
		token = strtok(NULL, " ");
		}
cmd_argv[i] = NULL;

if (cmd_argv[0] == NULL; /*terminer le tableau avec NULL*/
continue;
if (handle_exit(cmd_argv, argv) != -1)
break;
execute_command(cmd_arg, arv);
}
        execute_command(cmd-argv, argv); /* Exécute la commande */
    }

    free(line); /* Libère la mémoire */
    return (0);
}

/**
* handle_exit - gere la commande exit
* @cmd_argv: array d'args
* @argv: vecteur d'arg du main
* Return: -1 if exit is not found or exit code
*/

int handle_exit(char **cmd_argv, char **argv)
{
int exit_code;

	if (strcmp(cmd_argv[0], "exit") != 0) /*verifie si cmd is exit*/
		return (-1);

	if (cmd_argv[1]) /*si code d'etat fourni*/
		{
		exit_code = parse_exit_code(cmd_argv[1], argv);
		if (exit_code == -1) /*erreur de parsing*/
			return (2); /*code erreur pour arg invalide*/
		if (cmd_argv[2]) /*si trop d'args*/
			{
			fprintf(stderr, "%s: exit: too many arguments\n",argv[0]);
			return (-1); /*ne pas quitter*/
			}
		}
	else
		exit_code = 0;
	exit(exit_code);
}

/**
* parse_exit_code - parses the exit code arg
* @arg: argument to parse
* @argv: vecteur d'arg du main
* Return: parsed exit code, or -1 if invalid
*/

int parse_exit_code(const char *arg, char **argv)
{
int exit_code = 0;
int i;

for (i = 0; arg[i] != '\0'; i++)
	{
	if (arg[i] < '0' || arg[i] > '9')
		{
		fprintf(stderr, "%s: exit: %s: numeric arg required\n", argv[0], arg);
		return (-1);
		}
	exit_code = exit_code * 10 + (arg[i] - '0');
	}
return (exit_code);
}

/**
* display_prompt - affiche le shell prompt
*/

void display_prompt(void)
{
printf("($) ");
fflush(stdout); /*assure que le prompt est affiché immedatemment*/
}

void execute_command(char *line, char **argv) {
    pid_t pid;
    int status;
    char *cmd_argv[100];
    char *full_command;
    int i = 0;
    char *token;

    if (line[0] == '\0')
        return;

    // Tokenize the line into command and arguments
    token = strtok(line, " ");
    while (token != NULL) {
        cmd_argv[i++] = token;
        token = strtok(NULL, " ");
    }
    cmd_argv[i] = NULL;

    pid = fork();
    if (pid == -1) {
        perror("Erreur fork");
        return;
    }

    if (pid == 0) { /* Child process */
        full_command = find_command_in_path(cmd_argv[0]); // Use the isolated function
        if (full_command == NULL) {
            fprintf(stderr, "%s: command not found\n", cmd_argv[0]);
            exit(EXIT_FAILURE);
        }

        if (execve(full_command, cmd_argv, environ) == -1) {
            perror("Execve");
            exit(EXIT_FAILURE);
        }
    } else if (pid > 0) { /* Parent process */
        wait(&status);
    }
}
