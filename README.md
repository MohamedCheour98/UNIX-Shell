## UNIX-Shell

These are three seperate C programs where each simulates a UNIX command line.

The files below allow users to do the following functionalities:

1. The find file:
- Locate files and directories.
- Search a text segment within files.
- List running processes.
- Quit a running program.

2. The pipe file:
- Redirect the output of one command/program/process to another command/program/process for further processing.

3. The pwd (Print Working Directory) file:
- Print the path of the working directory, starting from the root.  
NB: This program does not use the getcwd(3) function by POSIX. It traverses the directories and gets the current working directory from scratch.
