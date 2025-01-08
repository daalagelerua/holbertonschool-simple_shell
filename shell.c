#include "shell.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

extern char **environ; /* Déclaration de la variable globale environ */

/* Prototypes */
void display_prompt(void);
void execute_command(char *command, char **argv);

int main(int argc, char **argv)
{
    char *line = NULL;    /* Pointeur pour stocker l'entrée utilisateur */
    size_t len = 0;       /* Taille de la mémoire allouée à line */
    ssize_t nread;        /* Nombre de caractères lus */
    int is_interactive = isatty(STDIN_FILENO); /* Vérifie si entrée interactive */

    (void)argc; /* argc n'est pas utilisé donc on le mute */

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

        if (line[0] == '\0') /* Si aucune commande, continuer */
            continue;

        execute_command(line, argv); /* Exécuter la commande */
    }

    free(line);
    return (0);
}

/**
 * display_prompt - Affiche le shell prompt
 */
void display_prompt(void)
{
    printf("$ ");
    fflush(stdout); /* Assure que le prompt est affiché immédiatement */
}

/**
 * execute_command - Crée les forks et exécute les commandes
 * @command: Commande utilisateur
 * @argv: Vecteur d'argument de la fonction main
 */
void execute_command(char *command, char **argv)
{
    pid_t pid;
    int status;

    pid = fork();
    if (pid == -1)
    {
        perror("Erreur fork");
        return;
    }

    if (pid == 0) /* Processus enfant */
    {
  char *cmd_argv[2]; /* Déclare le tableau */
        cmd_argv[0] = command; /* Affecte la commande */
        cmd_argv[1] = NULL;    /* Terminateur NULL */
        if (execve(command, cmd_argv, environ) == -1) /* Vérifie la commande */
        {
            fprintf(stderr, "%s: %s: No such file or directory\n", argv[0], command);
            exit(EXIT_FAILURE);
        }
    }
    else /* Processus parent */
    {
        wait(&status);
    }
}
