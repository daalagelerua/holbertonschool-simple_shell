#include "shell.h"

/**
* main - boucle principale et affichage du prompt
* @argc: compteur d'arguments (non utilisé ici)
* @argv: vecteur d'arguments (contient nom du programme ou arguments)
* Return: 0
*/

int main(int argc __attribute__((unused)), char **argv)
{
char *line = NULL; /* Pointeur pour stocker l'entrée utilisateur */
size_t len = 0; /* Taille de la mémoire allouée à line */
ssize_t nread; /* Nombre de caractères lus */
int i = 0, is_interactive = isatty(STDIN_FILENO); /*Vérif entrée interact*/
char *token, *cmd_argv[100];

while (1) /* Boucle infinie pour garder le shell actif */
	{
	if (is_interactive)
	display_prompt(); /* Affiche le prompt seulement en mode interactif */
	nread = getline(&line, &len, stdin); /* Lit l'entrée utilisateur */
	if (nread == -1) /* Vérifie si EOF ou erreur */
		{
		if (is_interactive)
		printf("\n"); /* Affiche une nouvelle ligne avant de quitter */
		break;
		}
	line[nread - 1] = '\0'; /* Supprime le \n en fin de commande */
	if (strcmp(line, "exit") == 0) /* Vérifie si la commande est 'exit' */
		{
		handle_exit(line);
		}
	token = strtok(line, " "); /*decoupe chaine en token a chaque delimiteur*/
	while (token != NULL)
		{
		cmd_argv[i++] = token; /*ajoute chaque token au tableau cmd_argv*/
		token = strtok(NULL, " ");
		}
	cmd_argv[i] = NULL; /* Termine le tableau avec NULL */
	i = 0;
if (cmd_argv[0] == NULL) /* Si aucune commande, continuer */
		continue;
	if (strcmp(cmd_argv[0], "env") == 0) /* Vérifie si commande est 'env' */
		{
		handle_env();
		continue; /* Retourne à la boucle principale */
		}
	execute_command(cmd_argv, argv[0]); /* Exécute la commande */
	}
free(line); /* Libère la mémoire allouée à line */
return (0);
}

/**
 * handle_env - Affiche les variables d'environnement
 */

void handle_env(void)
{
/*copie du ptr 'environ' pour iterer sur les variables sans modif l'original*/
char **env = environ;

while (*env) /*tant que env n'est pas NULL*/
	{
	printf("%s\n", *env); /*affiche la chaine de caractere pointée*/
	env++; /*passe a la chaine suivante*/
	}
}

/**
* handle_exit - Gere la commande exit
* @line: ligne dentree utilisateur
*/

void handle_exit(char *line)
{
    free(line); /* Libère la mémoire allouée */
    exit(0);    /* Quitte le programme avec un code de succès */
}

/**
* display_prompt - affiche le shell prompt
*/

void display_prompt(void)
{
	printf("($) ");
	fflush(stdout); /* Assure que le prompt est affiché immédiatement */
}
