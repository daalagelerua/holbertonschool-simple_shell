#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shell.h"

char *find_command_in_path(char *command)
{
    char *path = getenv("PATH");
    char *path_copy, *dir, *full_path;
    size_t len;

    if (!path)
        return NULL;

    path_copy = strdup(path);  // Crée une copie du PATH
    if (!path_copy)
        return NULL;

    dir = strtok(path_copy, ":");
    while (dir != NULL)
    {
        len = strlen(dir) + strlen(command) + 2;  // Calcul de la longueur totale
        full_path = malloc(len);
        if (!full_path)
        {
            free(path_copy);
            return NULL;
        }

        snprintf(full_path, len, "%s/%s", dir, command);  // Crée le chemin complet

        if (access(full_path, X_OK) == 0)  // Vérifie si la commande est exécutable
        {
            free(path_copy);
            return full_path;  // Retourne le chemin complet de la commande
        }

        free(full_path);
        dir = strtok(NULL, ":");  // Passe au répertoire suivant dans le PATH
    }

    free(path_copy);
    return NULL;  // Retourne NULL si la commande n'est pas trouvée
}
