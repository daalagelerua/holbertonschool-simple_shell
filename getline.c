#include <stdio.h>
#include <stdlib.h>

/**
* main - boucle principale et affichage du prompt
*
* Return: 0
*/

int main(void)
{
char *line = NULL; /*pointeur pour stocker l'entrée utilisateur*/
size_t len = 0; /*taille de la mémoire alloué à line*/
ssize_t nread; /*nombre de caractère lus*/

while (1) /*boucle infinie pour garder le shell actif*/
	{
	printf("($) "); /*affiche le prompt*/
	nread = getline(&line, &len, stdin); /*lit l'entrée utilisateur*/

	if (nread == -1) /*verifie si EOF ou erreur*/
		{
		printf("\n"); /*affiche une nouvelle ligne avant de quitter*/
		break;
		}

	printf("Vous avez tapé : %s, line); /*affiche la commande saisie*/
	}
free(line); /*libère la mémoire*/
return(0);
}
