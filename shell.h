#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void display_prompt(void);
void execute_command(char **cmd_argv, char **argv);
int handle_exit(char **cmd_argv, char **argv);
int parse_exit_code(const char *arg, char **argv);
char *find_command_path(char *command);

#endif
