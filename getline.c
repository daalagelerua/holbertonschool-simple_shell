#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

/**
* main - boucle principale et affichage du prompt
*
* Return: 0
*/

int main(void)
{
char *line = NULL; /*pointeur pour stocker l'entrée utilisateur*/
char *argv[2];
size_t len = 0; /*taille de la mémoire alloué à line*/
ssize_t nread; /*nombre de caractère lus*/
pid_t pid;
int status;

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
		argv[0] = line; /*commande a executer*/
		argv[1] = NULL; /*fin de l'argument*/

		if (execve(line, argv, NULL) == -1)
			{
			perror(line); /*affiche une erreur si execve echoue*/
			exit(EXIT_FAILURE); /*quitte le process enfant*/
			}
		}
	else /*process parent*/
		wait(&status); /*attend que l'enfant se termine*/
	}
free(line); /*libère la mémoire*/
return(0);
}
