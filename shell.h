#ifndef SHELL_H
#define SHELL_H

void display_prompt(void);
void execute_command(char *line, char **argv);
int main(int argc, char **argv);

#endif
