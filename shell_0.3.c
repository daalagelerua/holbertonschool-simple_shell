#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "shell.h"
#define PROMPT "($) "
#define DELIM " \n"


extern char **environ;
char *find_command_in_path(char *command);
void execute_command(char *line, char **argv);

int main(int argc, char **argv)
{
    char *line = NULL;
    size_t len = 0;
    ssize_t nread;
    int is_interactive = isatty(STDIN_FILENO);

    (void)argc;

    while (1)
    {
        if (is_interactive)
            printf("($) ");
        nread = getline(&line, &len, stdin);

        if (nread == -1)
        {
            if (is_interactive)
                printf("\n");
            break;
        }

        line[nread - 1] = '\0';

        if (strcmp(line, "exit") == 0)
            break;

        execute_command(line, argv);
    }

    free(line);
    return 0;
}

void execute_command(char *line, char **argv)
{
    pid_t pid;
    int status;
    char *cmd_argv[100];
    char *full_command;
    int i = 0;
    char *token;

    // Tokenisation de la ligne d'entrée
    if (line[0] == '\0')
        return;

    token = strtok(line, " ");
    while (token != NULL)
    {
        cmd_argv[i++] = token;
        token = strtok(NULL, " ");
    }
    cmd_argv[i] = NULL;

    if (cmd_argv[0] == NULL)
        return;  // Commande vide, on ne fait rien

    pid = fork();
    if (pid == -1)
    {
        perror("Erreur fork");
        return;
    }

    if (pid == 0)  // Processus fils
    {
        // Vérifier si la commande a un chemin absolu (contient un '/')
        if (strchr(cmd_argv[0], '/') != NULL)
        {
            full_command = strdup(cmd_argv[0]);  // Utiliser le chemin complet directement
        }
        else  // Sinon, chercher la commande dans le PATH
        {
            full_command = find_command_in_path(cmd_argv[0]);
            if (full_command == NULL)
            {
                fprintf(stderr, "%s: command not found\n", cmd_argv[0]);
                exit(EXIT_FAILURE);
            }
        }

        // Exécuter la commande avec execve
        if (execve(full_command, cmd_argv, environ) == -1)
        {
            perror("Execve échoué");
            exit(EXIT_FAILURE);
        }
    }
    else if (pid > 0)  // Processus parent
    {
        wait(&status);  // Attendre la fin du processus fils
    }
}
