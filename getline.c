#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

/**
* main - boucle principale et affichage du prompt
* @argc: compteur d'argument (non utilisé ici)
* @argv: vecteur d'argument (contien nom du programme ou arguments
* Return: 0
*/

int main(int argc, char **argv)
{
char *line = NULL; /*pointeur pour stocker l'entrée utilisateur*/
char *cmd_argv[2];
size_t len = 0; /*taille de la mémoire alloué à line*/
ssize_t nread; /*nombre de caractère lus*/
pid_t pid;
int status;

(void)argc; /*argc n'est pas utilisé donc on le mute*/

while (1) /*boucle infinie pour garder le shell actif*/
	{
	printf("($) "); /*affiche le prompt*/
	nread = getline(&line, &len, stdin); /*lit l'entrée utilisateur*/

	if (nread == -1) /*verifie si EOF ou erreur*/
		{
		printf("\n"); /*affiche une nouvelle ligne avant de quitter*/
		break;
		}

	line[nread - 1] = '\0'; /*supprime le \n en fin de commande*/

	if (strcmp(line, "exit") == 0)
		break;

	pid = fork(); /*crée un process enfant*/
	if (pid == -1)
		{
		perror("erreur fork");
		continue;
		}

	if (pid == 0) /*process enfant*/
		{
		cmd_argv[0] = line; /*commande a executer*/
		cmd_argv[1] = NULL; /*fin de l'argument*/

		if (execve(line, cmd_argv, NULL) == -1)
			{
			/*affiche erreur avec nom du prog*/
			fprintf(stderr, "%s: 1: %s: not found\n", argv[0], line);
			exit(EXIT_FAILURE); /*quitte le process enfant*/
			}
		}
	else /*process parent*/
		wait(&status); /*attend que l'enfant se termine*/
	}
free(line); /*libère la mémoire*/
return(0);
}
