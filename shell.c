#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

extern char **environ;

void display_prompt(void);
void execute_command(char *command, char **argv);

/**
 * main - Entry point of the shell program
 * @argc: Argument count (unused)
 * @argv: Argument vector, contains the program name and possible arguments
 *
 * Description: This function implements a UNIX command line interpreter.
 * It displays a prompt in interactive mode, reads a command from the user,
 * and executes it. The shell runs in an infinite loop until the user
 * inputs EOF (Ctrl+D) or an error occurs.
 *
 * Return: Always 0 (Success)
 */
int main(int argc, char **argv)
{
    char *line = NULL;
    size_t len = 0;
    ssize_t nread;
    int is_interactive = isatty(STDIN_FILENO);

    (void)argc; /* Unused parameter */

    while (1)
    {
        if (is_interactive)
            display_prompt();

        nread = getline(&line, &len, stdin);
        if (nread == -1)
        {
            if (is_interactive)
                printf("\n");
            break;
        }

        /* Remove trailing newline */
        if (line[nread - 1] == '\n')
            line[nread - 1] = '\0';

        /* Ignore empty lines */
        if (line[0] == '\0')
            continue;

        execute_command(line, argv);
    }

    free(line); /* Free allocated memory for line */
    return (0);
}

/**
 * display_prompt - Displays the shell prompt
 */
void display_prompt(void)
{
    printf("$ ");
    fflush(stdout);
}

/**
 * execute_command - Executes a command with no arguments
 * @command: The command to execute
 * @argv: Argument vector from main
 *
 * Description: This function creates a child process to execute the command.
 * If the command is not found or fails to execute, it prints an error message.
 */
void execute_command(char *command, char **argv)
{
    pid_t pid;
    int status;

    pid = fork();
    if (pid == -1)
    {
        perror("Error:");
        return;
    }

    if (pid == 0)
    {
        /* Execute command using execvp to search in PATH */
        char *cmd_argv[2];
        cmd_argv[0] = command;
        cmd_argv[1] = NULL;

        if (execvp(command, cmd_argv) == -1)
        {
            fprintf(stderr, "%s: %s: command not found\n", argv[0], command);
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        /* Wait for the child process to finish */
        wait(&status);
    }
}
