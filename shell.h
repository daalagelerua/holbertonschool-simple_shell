#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <errno.h>

extern char **environ;

void display_prompt(void);
void execute_command(char **cmd_argv, char *argv);
char *find_command_path(char *command);
void handle_env(void);
void handle_exit(char *line);

#endif
