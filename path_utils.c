#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shell.h"

char *find_command_in_path(char *command) {
    if (command[0] == '/') {  // Si la commande commence par un '/', c'est un chemin absolu
        if (access(command, X_OK) == 0) {
            return command;  // La commande est déjà complète, on vérifie juste l'accès
        }
        return NULL;  // Si la commande n'est pas trouvée, on retourne NULL
    }

    // Sinon, on cherche dans les répertoires du PATH
    char *path = getenv("PATH");
    char *path_copy = strdup(path);
    char *dir = strtok(path_copy, ":");
    char *full_path = malloc(1024);  // Allocation dynamique pour éviter le problème du 'static'

    if (!full_path) {
        perror("Erreur d'allocation mémoire");
        free(path_copy);
        return NULL;
    }

    while (dir != NULL) {
        snprintf(full_path, 1024, "%s/%s", dir, command);
        if (access(full_path, X_OK) == 0) {
            free(path_copy);
            return full_path;
        }
        dir = strtok(NULL, ":");
    }

    free(path_copy);
    free(full_path);
    return NULL;
}
