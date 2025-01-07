#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "shell.h"

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

    if (line[0] == '\0')
        return;

    token = strtok(line, " ");
    while (token != NULL) {
        if (strcmp(token, "<") == 0) {
            input_redirect = 1;
            token = strtok(NULL, " ");
            input_file = token;
        } else if (strcmp(token, ">") == 0) {
            output_redirect = 1;
            token = strtok(NULL, " ");
            output_file = token;
        } else {
            cmd_argv[i++] = token;
        }
        token = strtok(NULL, " ");
    }
    cmd_argv[i] = NULL;

    if (cmd_argv[0] == NULL)
        return;

    pid = fork();
    if (pid == -1) {
        perror("Erreur fork");
        return;
    }

    if (pid == 0) {
        if (input_redirect) {
            int fd = open(input_file, O_RDONLY);
            if (fd == -1) {
                perror("Erreur d'ouverture du fichier d'entrée");
                exit(EXIT_FAILURE);
            }
            dup2(fd, STDIN_FILENO);
            close(fd);
        }

        if (output_redirect) {
            int fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd == -1) {
                perror("Erreur d'ouverture du fichier de sortie");
                exit(EXIT_FAILURE);
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }

        full_command = find_command_in_path(cmd_argv[0]);
        if (full_command == NULL) {
            fprintf(stderr, "%s: command not found\n", cmd_argv[0]);
            exit(EXIT_FAILURE);
        }

        if (execve(full_command, cmd_argv, environ) == -1) {
            perror("Execve échoué");
            exit(EXIT_FAILURE);
        }
    } else if (pid > 0) {
        wait(&status);
    }
}
