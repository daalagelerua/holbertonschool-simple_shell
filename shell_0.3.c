#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

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

void execute_command(char *line, char **argv)
{
    pid_t pid;
    int status;
    char *cmd_argv[100];  // Tableau pour les arguments de la commande
    char *full_command;
    int i = 0;
    char *token;

    if (line[0] == '\0')  // Si la ligne est vide, on ne fait rien
        return;

    // Tokenize la ligne en commandes et arguments
    token = strtok(line, " ");
    while (token != NULL)
    {
        cmd_argv[i++] = token;  // Ajouter l'argument au tableau
        token = strtok(NULL, " ");
    }
    cmd_argv[i] = NULL;  // Terminer le tableau des arguments avec NULL

    // Vérifier si la commande existe dans le PATH avant de fork
    full_command = find_command_in_path(cmd_argv[0]);  // Trouver le chemin complet de la commande

    if (full_command == NULL)
    {
        // Si la commande n'existe pas, afficher un message d'erreur et ne pas appeler fork()
        fprintf(stderr, "%s: command not found\n", cmd_argv[0]);
        return;  // Sortir de la fonction sans fork()
    }

    // Fork pour exécuter la commande
    pid = fork();
    if (pid == -1)
    {
        perror("Erreur fork");
        return;  // Erreur fork
    }

    if (pid == 0)  /* Processus enfant */
    {
        // Vérifier que la commande peut être exécutée
        if (execve(full_command, cmd_argv, environ) == -1)
        {
            perror("Execve");
            exit(EXIT_FAILURE);  // En cas d'échec d'exécution
        }
    }
    else if (pid > 0)  /* Processus parent */
    {
        // Attendre la fin du processus enfant
        wait(&status);
    }
}
