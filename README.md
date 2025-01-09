# holbertonschool-simple_shell

## Project Objective
The goal of this project is to create a simple UNIX command interpreter (shell).

## Title

C- Simple Shell

### Files Description

- `README.md`

Contains a detailed description of the project, including its purpose, functionality, usage instructions, and any specific features or limitations.

- `exec.c`

This file contains the implementation of the execution function.
The function is responsible for executing user-entered commands by using system calls like execve().

- `path.c`

This file contains the implementation of the path resolution function.
It handles locating executable files by searching through directories specified in the PATH environment variable.

- `man_1_simple_shell`

The manual page for the project, written in a format compatible with the UNIX man command.
Provides detailed documentation on how to use the shell, including available commands and options.

- `AUTHORS`

A file listing the authors or contributors to the project.
It typically contains names, email addresses, or other relevant contact information.

- `shell.c`

This file contains the main code for the shell program.
It includes the core functionality, such as the input loop, command parsing, and invocation of the execution or path functions.

- `shell.h`

The header file for the project.
It contains function prototypes, macros, and global variable declarations used across multiple source files in the project.

### Requirements:

Allowed editors: vi, vim, emacs
All your files will be compiled on Ubuntu 20.04 LTS using gcc, using the 
options -Wall -Werror -Wextra -pedantic -std=gnu89
All your files should end with a new line
A README.md file, at the root of the folder of the project is mandatory
Your code should use the Betty style. 
It will be checked using betty-style.pl and betty-doc.pl
Your shell should not have any memory leaks
No more than 5 functions per file
All your header files should be include guarded
Use system calls only when you need to (why?)

### List of allowed functions and system calls+:

all functions from string.h  
access (man 2 access)
chdir (man 2 chdir)
close (man 2 close)
closedir (man 3 closedir)
execve (man 2 execve)
exit (man 3 exit)
_exit (man 2 _exit)
fflush (man 3 fflush)
fork (man 2 fork)
free (man 3 free)
getcwd (man 3 getcwd)
getline (man 3 getline)
getpid (man 2 getpid)
isatty (man 3 isatty)
kill (man 2 kill)
malloc (man 3 malloc)
open (man 2 open)
opendir (man 3 opendir)
perror (man 3 perror)
printf (man 3 printf)
fprintf (man 3 fprintf)
vfprintf (man 3 vfprintf)
sprintf (man 3 sprintf)
putchar (man 3 putchar)
read (man 2 read)
readdir (man 3 readdir)
signal (man 2 signal)
stat (__xstat) (man 2 stat)
lstat (__lxstat) (man 2 lstat)
fstat (__fxstat) (man 2 fstat)
strtok (man 3 strtok)
wait (man 2 wait)
waitpid (man 2 waitpid)
wait3 (man 2 wait3)
wait4 (man 2 wait4)
write (man 2 write)

### Tasks:

0 - README.md, man, authors.
1 - Betty style.
2 - Simple shell 0.1 - write a UNIX command line interpreter.
3 - Simple shell 0.2 - handles command lines with arguments.
4 - Simple shell 0.3 - handle path, fork must not be called if the command doesn't exist.
5 - Simple shell 0.4 - implement exit built-in.
6 - Simple shell 0.5 - implement the end built-in.

### Compilation:

Your shell will be compiled this way:

```bash
gcc -Wall -Werror -Wextra -pedantic -std=gnu89 *.c -o hsh
```
