#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>  // Pour open()
#include "shell.h"

extern char **environ;  // Déclaration de l'environnement global

// Déclaration de la fonction pour trouver une commande dans le PATH
char *find_command_in_path(char *command);

// Fonction d'exécution de la commande
void execute_command(char *line, char **argv);

int main(int argc, char **argv)
{
    char *line = NULL;  // Pointeur pour stocker l'entrée utilisateur
    size_t len = 0;  // Taille de la mémoire allouée à line
    ssize_t nread;  // Nombre de caractères lus
    int is_interactive = isatty(STDIN_FILENO);  // Vérifie si entrée interactive

    (void)argc;  // argc n'est pas utilisé donc on le mute

    while (1)  // Boucle infinie pour garder le shell actif
    {
        if (is_interactive)
            printf("($) ");  // Affiche le prompt seulement en mode interactif
        nread = getline(&line, &len, stdin);  // Lit l'entrée utilisateur

        if (nread == -1)  // Vérifie si EOF ou erreur
        {
            if (is_interactive)
                printf("\n");  // Affiche une nouvelle ligne avant de quitter
            break;
        }

        line[nread - 1] = '\0';  // Supprime le '\n' en fin de commande

        if (strcmp(line, "exit") == 0)
            break;

        execute_command(line, argv);  // Exécute la commande
    }

    free(line);  // Libère la mémoire
    return 0;
}

void execute_command(char *line, char **argv) {
    pid_t pid;
    int status;
    char *cmd_argv[100];
    char *full_command;
    int i = 0;
    char *token;
    char *input_file = NULL;
    char *output_file = NULL;
    int input_redirect = 0;
    int output_redirect = 0;

    if (line[0] == '\0')  // Si la ligne est vide, on ne fait rien
        return;

    // Tokenize la ligne en commandes et arguments
    token = strtok(line, " ");
    while (token != NULL) {
        // Vérifie si c'est une redirection
        if (strcmp(token, "<") == 0) {
            input_redirect = 1;
            token = strtok(NULL, " ");
            input_file = token;  // L'argument suivant est le fichier pour l'entrée
        } else if (strcmp(token, ">") == 0) {
            output_redirect = 1;
            token = strtok(NULL, " ");
            output_file = token;  // L'argument suivant est le fichier pour la sortie
        } else {
            cmd_argv[i++] = token;
        }
        token = strtok(NULL, " ");
    }
    cmd_argv[i] = NULL;

    if (cmd_argv[0] == NULL)  // Si la commande est vide, on sort
        return;

    // Création du processus fils
    pid = fork();
    if (pid == -1) {
        perror("Erreur fork");
        return;
    }

    if (pid == 0) {  // Processus fils
        // Gestion des redirections
        if (input_redirect) {  // Redirection d'entrée
            int fd = open(input_file, O_RDONLY);
            if (fd == -1) {
                perror("Erreur d'ouverture du fichier d'entrée");
                exit(EXIT_FAILURE);
            }
            dup2(fd, STDIN_FILENO);  // Redirige l'entrée standard
            close(fd);
        }

        if (output_redirect) {  // Redirection de sortie
            int fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd == -1) {
                perror("Erreur d'ouverture du fichier de sortie");
                exit(EXIT_FAILURE);
            }
            dup2(fd, STDOUT_FILENO);  // Redirige la sortie standard
            close(fd);
        }

        // Trouve la commande complète dans le PATH
        full_command = find_command_in_path(cmd_argv[0]);
        if (full_command == NULL) {
            fprintf(stderr, "%s: command not found\n", cmd_argv[0]);
            exit(EXIT_FAILURE);
        }

        // Exécute la commande
        if (execve(full_command, cmd_argv, environ) == -1) {
            perror("Execve échoué");
            exit(EXIT_FAILURE);
        }
    } else if (pid > 0) {  // Processus parent
        wait(&status);  // Attend que le fils termine
    }
}
