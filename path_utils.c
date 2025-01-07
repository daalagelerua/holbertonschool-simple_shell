#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shell.h"

char *find_command_in_path(char *command) {
    if (command[0] == '/') {
        if (access(command, X_OK) == 0) {
            return command;
        }
        return NULL;
    }

    char *path = getenv("PATH");
    char *path_copy = strdup(path);
    char *dir = strtok(path_copy, ":");
    char *full_path = malloc(1024);

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
