.TH SIMPLE-SHELL 1 "January 2025" "1.0" "Manual page for HSH (Holberton Simple Shell)"
.SH NAME
hsh \- a simple Unix command-line interpreter
.SH SYNOPSIS
.B hsh
[\fIoptions\fR]
.SH DESCRIPTION
.B hsh
is an interactive and non-interactive shell developed by Aurèle et Joséphine.  
It allows users to execute system commands by reading user input, parsing the commands, and running them in child processes.

The shell operates in two modes:  
- **Interactive mode**: Displays a prompt and waits for user input.  
- **Non-interactive mode**: Reads and executes commands from a file or a pipeline.

This shell was implemented as part of the Holberton School curriculum to learn the fundamentals of system calls and process management.

.SH COMMANDS
.TP
.B exit
Exit the shell.  
Usage:  
.B exit  
This command terminates the shell.
.TP
.B env
Print the current environment variables.  
Usage:  
.B env
.TP
.B cd [directory]
Change the current working directory to \fIdirectory\fR. If no directory is provided, the shell defaults to the user's home directory.  
Usage:  
.B cd /path/to/directory
.TP
.B help
Display a list of built-in commands and their usage.  
Usage:  
.B help

.SH EXAMPLES
.PP
Start the shell in interactive mode:
.B ./hsh
.PP
Execute a system command:
.B ls -l
.PP
Run a command in non-interactive mode:
.B echo "/bin/ls" | ./hsh
.PP
Exit the shell:
.B exit
.PP
Terminate with \fICTRL+D\fR:
.B ^D

.SH PATH
The shell uses the \fIPATH\fR environment variable to locate executables. When a command is typed, the shell searches the directories in \fIPATH\fR for the corresponding binary. If the binary is not found, an error message is displayed.

.SH SYSTEM CALLS
The following system calls are used in \fBhsh\fR:
.TP
.B fork
Creates a new process.
.TP
.B execve
Replaces the current process with a new program.
.TP
.B wait
Suspends the parent process until a child process terminates.
.TP
.B getpid
Retrieves the process ID of the current process.
.TP
.B getppid
Retrieves the parent process ID.

.SH ENVIRONMENT
The shell can manipulate environment variables using:
.TP
.B getenv
Retrieve the value of an environment variable.
.TP
.B setenv
Set or update an environment variable.
.TP
.B unsetenv
Remove an environment variable.

.SH FILES
.TP
.B AUTHORS
Contains the list of contributors to the project: Joséphine and Aurele.
.TP
.B README.md
Overview and usage instructions for the shell.
.TP
.B man_1_simple_shell
This manual page.

.SH COPYRIGHT
Copyright (C) 2025 Aurele and Joséphine. All rights reserved.

.SH SEE ALSO
.BR sh (1),
.BR bash (1),
.BR execve (2),
.BR fork (2),
.BR wait (2)

.SH AUTHOR
Written by Aurele and Josephine.
