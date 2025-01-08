
#include "shell.h"
/**
* find_command_path - finds the full path of a command
* @command: command to search for
* Return: full path to the command
*/
char *find_command_path(char *command)
{
char *path, *dir, *full_path;
size_t command_len;
path = getenv("PATH");
if (!path)
	return (NULL);
command_len = strlen(command);
path = strdup(path);
if (!path)
	return (NULL);
dir = strtok(path, ":");
while (dir)
	{
	full_path = malloc(strlen(dir) + command_len + 2);
	if (!full_path)
		{
		free(path);
		return (NULL);
		}
	sprintf(full_path, "%s/%s", dir, command);
	if (access(full_path, X_OK) == 0)
		{
		free(path);
		return (full_path);
		}
	free(full_path);
	dir = strtok(NULL, ":");
	}
free(path);
return (NULL);
}
