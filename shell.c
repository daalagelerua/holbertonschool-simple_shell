#include "shell.h"
#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

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
(void)argc;
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
line[nread - 1] = '\0';
if (line[0] == '\0')
continue;
execute_command(line, argv);
}
free(line);
return (0);
}
/**
 * display_prompt - Displays the shell prompt
 *
 * Description: This function prints the shell prompt (`$ `) to the standard
 * output. It ensures that the prompt is immediately displayed by flushing
 * the output buffer.
 *
 * Return: void
 */
void display_prompt(void)
{
printf("$ ");
fflush(stdout);
}

/**
 * execute_command - Executes a given command using a child process
 * @command: The command to execute (full path required)
 * @argv: The argument vector of the main function
 *
 * Description: This function creates a child process using `fork`. The child
 * process executes the specified command using `execve`, while the parent
 * process waits for the child to finish. If the command is not found or
 * fails to execute, an error message is printed.
 *
 * Return: void
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
if (pid == 0)
{
char *cmd_argv[2];
cmd_argv[0] = command;
cmd_argv[1] = NULL;
if (execve(command, cmd_argv, environ) == -1)
{
fprintf(stderr, "%s: %s: No such file or directory\n", argv[0], command);
exit(EXIT_FAILURE);
}
}
else
{
wait(&status);
}
}
