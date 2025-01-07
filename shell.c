#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

extern char **environ;

/*protos*/
void display_prompt(void);
void execute_command(char *line, char **argv);
int handle_exit(char **cmd_argv, char **argv);
int parse_exit_code(const char *arg, char **argv);

/**
* main - boucle principale et affichage du prompt
* @argc: compteur d'argument (non utilisé ici)
* @argv: vecteur d'argument (contien nom du programme ou arguments
* Return: 0
*/

int main(int argc, char **argv)
{
char *line = NULL; /*pointeur pour stocker l'entrée utilisateur*/
size_t len = 0; /*taille de la mémoire alloué à line*/
ssize_t nread; /*nombre de caractère lus*/
int is_interactive = isatty(STDIN_FILENO); /*verifie si entrée interactive*/
char *cmd_argv[100];
int i = 0;
char *token;

(void)argc; /*argc n'est pas utilisé donc on le mute*/

while (1) /*boucle infinie pour garder le shell actif*/
	{
	if (is_interactive)
		display_prompt(); /*affiche le prompt seulement en mode interactif*/
	nread = getline(&line, &len, stdin); /*lit l'entrée utilisateur*/

	if (nread == -1) /*verifie si EOF ou erreur*/
		{
		if (is_interactive)
			printf("\n"); /*affiche une nouvelle ligne avant de quitter*/
		break;
		}

	line[nread - 1] = '\0'; /*supprime le \n en fin de commande*/

	token = strtok(line, " ");
	while (token != NULL)
		{
		cmd_argv[i++] = token;
		token = strtok(NULL, " ");
		}
	cmd_argv[i] = NULL; /*terminer le tableau avec NULL*/

	if (cmd_argv[0] == NULL) /*si aucune commande continuer*/
		continue;

	if (handle_exit(cmd_argv, argv) != -1)
		break;

	execute_command(cmd_argv, argv);
	}
free(line);
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
			exit_code = parse_exit_code(cmd_argv[1], argv)
			if (exit_code == -1) /*erreur de parsing*/
				return (2); /*code erreur pour arg invalide*/
			if (cmd_argv[2]) /*si trop d'args*/
				{
				fprintf(stderr, %s: exit: too many arguments\n",argv[0]);
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

/**
* execute_command - crée les forks et execute les commandes
* @line: entrée utilisateur
* @argv: vecteur d'argument de la fonction main
*/

void execute_command(char *line, char **argv)
{
pid_t pid;
int status;
char *cmd_argv[2];

if (line[0] == '\0')
	return;

pid = fork();
if (pid == -1)
	{
	perror("erreur fork");
	return;
	}

if (pid == 0) /*process enfant*/
	{
	if (access(line, X_OK) == -1) /*verifie la commande*/
		{
		fprintf(stderr, "%s: No such file or directory\n", argv[0]);
		exit(EXIT_FAILURE);
		}

	cmd_argv[0] = line;
	cmd_argv[1] = NULL;

	if (execve(line, cmd_argv, environ) == -1)
		{
		fprintf(stderr, "%s: No such file or directory\n", argv[0]);
		exit(EXIT_FAILURE);
		}
	}

if (pid < 0) /*process parent*/
	wait(&status);
}
