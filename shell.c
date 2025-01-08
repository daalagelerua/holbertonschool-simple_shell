#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

extern char **environ; /* Accès aux variables d'environnement */
void handle_env(void);
void handle_exit(char *line);

/**
 * main - boucle principale et affichage du prompt
 * @argc: compteur d'arguments (non utilisé ici)
 * @argv: vecteur d'arguments (contient nom du programme ou arguments)
 * Return: 0
 */
int main(int argc, char **argv)
{
    char *line = NULL; /* Pointeur pour stocker l'entrée utilisateur */
    size_t len = 0;    /* Taille de la mémoire allouée à line */
    ssize_t nread;     /* Nombre de caractères lus */
    int is_interactive = isatty(STDIN_FILENO); /* Vérifie si entrée interactive */
    char *cmd_argv[100];
    int i;
    char *token;

    (void)argc; /* argc n'est pas utilisé, donc on le mute */

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
	if (strcmp(line, "exit") == 0) /* Vérifie si commande est 'exit' */
        {
            handle_exit(line); /* Appel de la fonction exit */
        }
        i = 0;
        token = strtok(line, " ");
        while (token != NULL)
        {
            cmd_argv[i++] = token;
            token = strtok(NULL, " ");
        }
        cmd_argv[i] = NULL; /* Termine le tableau avec NULL */

        if (cmd_argv[0] == NULL) /* Si aucune commande, continuer */
            continue;

        if (strcmp(cmd_argv[0], "env") == 0) /* Vérifie si commande est 'env' */
        {
            handle_env();
            continue; /* Retourne à la boucle principale */
        }

        execute_command(cmd_argv, argv); /* Exécute la commande */
    }

    free(line); /* Libère la mémoire allouée à line */
    return (0);
}

/**
 * handle_env - Affiche les variables d'environnement
 */
void handle_env(void)
{
    char **env = environ;

    while (*env)
    {
        printf("%s\n", *env);
        env++;
    }
}

/**
 * handle_exit - Gère la commande exit
 * @line: ligne d'entrée utilisateur
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
