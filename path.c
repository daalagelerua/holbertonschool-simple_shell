#include "shell.h"

/**
* find_command_path - finds the full path of a command
* @command: command to search for
* Return: full path to the command
*/

char *find_command_path(char *command)
{
char *path, *dir, *full_path;
size_t command_len, buf_siz;
path = getenv("PATH");

if (!path || *path == '\0') /*si path n'existe pas ou est NULL*/
	return (NULL);
command_len = strlen(command); /*stocke la longueur du nom de la cmd*/
path = strdup(path); /*duplique path car strtok va modifier la chaine*/
if (!path)
	return (NULL);
dir = strtok(path, ":"); /*decoupage des repertoires*/
while (dir) /*parcourt chaque repertoire*/
	{ /*longueur repertoire + longueur commande*/
		/* +2 pour '/' entre rep et cmd et '\0'*/
	buf_siz = strlen(dir) + command_len + 2;
	full_path = malloc(buf_siz);
	if (!full_path)
		{
		free(path);
		return (NULL);
		}
	if ((snprintf(full_path, buf_siz, "%s/%s", dir, command)) >= (int)buf_siz)
		{
		fprintf(stderr, "error: buffer size insufficient for cmd path\n");
		free(full_path);
		free(path);
		return (NULL);
		}
	if (access(full_path, X_OK) == 0) /*full_path existe et est executable*/
		{
		free(path);
		return (full_path);
		}
	free(full_path); /*si access echoue ->free*/
	dir = strtok(NULL, ":"); /*passe au prochain rep*/
	}
free(path); /*si aucun rep ne contient cmd ->free*/
return (NULL);
}
