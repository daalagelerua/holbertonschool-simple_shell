#ifndef SHELL_H
#define SHELL_H

void display_prompt(void);
void execute_command(char *line, char **argv);
int main(int argc, char **argv);
char *find_command_in_path(char *command);
#endif
