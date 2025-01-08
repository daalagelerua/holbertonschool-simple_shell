#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

extern char **environ; /* Accès aux variables d'environnement */
void handle_env(void);

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

        if (handle_exit(cmd_argv, argv) != -1) /* Gère la commande exit */
            break;

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
 * @cmd_argv: array d'arguments
 * @argv: vecteur d'arguments du main
 * Return: -1 si exit n'est pas trouvé ou code de sortie
 */
int handle_exit(char **cmd_argv, char **argv)
{
    int exit_code;

    if (strcmp(cmd_argv[0], "exit") != 0) /* Vérifie si cmd est exit */
        return (-1);

    if (cmd_argv[1]) /* Si code d'état fourni */
    {
        exit_code = parse_exit_code(cmd_argv[1], argv);
        if (exit_code == -1) /* Erreur de parsing */
            return (2); /* Code erreur pour argument invalide */
        if (cmd_argv[2]) /* Si trop d'arguments */
        {
            fprintf(stderr, "%s: exit: too many arguments\n", argv[0]);
            return (-1); /* Ne pas quitter */
        }
    }
    else
        exit_code = 0;

    exit(exit_code); /* Quitte avec le code de sortie */
}

/**
 * parse_exit_code - Parse l'argument du code de sortie
 * @arg: argument à analyser
 * @argv: vecteur d'arguments du main
 * Return: code de sortie analysé, ou -1 si invalide
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
 * display_prompt - Affiche le shell prompt
 */
void display_prompt(void)
{
    printf("($) ");
    fflush(stdout); /* Assure que le prompt est affiché immédiatement */
}
