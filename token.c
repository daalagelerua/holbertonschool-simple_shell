#include "shell.h"

/**
* tokenize_input - separe une ligne d'entrée en tokens
* @line: la ligne entrée par l'utilisateur
* @cmd_argv: tableau de tokens
* Return: le nb de token
*/

int tokenize_input(char *line, char **cmd_argv)
{
int i = 0;
char *token;

token = strtok(line, " ");
while (token != NULL)
	{
	cmd_argv[i++] = token;
	token = strtok(NULL, " ");
	}
cmd_argv[i] = NULL;
return (i);
}
