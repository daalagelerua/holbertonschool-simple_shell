#include "shell.h"

/**
* find_command_path - finds the full path of a command
* @command: command to search for
* Return: full path to the command
*/

char *find_command_path(char *command)
{
 char *path = getenv("PATH");
    char *path_copy, *dir;
    char *full_path;

    if (!path)
        return (NULL);

    path_copy = strdup(path);
    if (!path_copy)
        return (NULL);

    dir = strtok(path_copy, ":");
    while (dir != NULL)
    {
        /* Allouer de la mémoire pour full_path */
        full_path = malloc(strlen(dir) + strlen(command) + 2);
        if (!full_path)
        {
            free(path_copy);
            return (NULL);
        }

        sprintf(full_path, "%s/%s", dir, command); /* Concaténer le chemin */

        if (access(full_path, X_OK) == 0) /* Vérifier l'accès exécutable */
        {
            free(path_copy);
            return (full_path); /* Retourner le chemin complet */
        }

        free(full_path); /* Libérer si non valide */
        dir = strtok(NULL, ":");
    }

    free(path_copy); /* Libérer la copie de PATH */
    return (NULL); /* Commande introuvable */
}
