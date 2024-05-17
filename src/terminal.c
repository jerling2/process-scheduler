/*
Joseph Erlinger (jerling2@uoregon.edu)

This file opens a new terminal (either xfce4 or gnome) to run the 
command: 'top -d0.1 -p1 -p2 -p3 ... -pN'. Note: the child process that runs
exec() performs a complicated launch procedure to launch the new terminal.
The new terminal instance is not a child of the MCP, resulting in the pid of
the terminal instance to be lost. The child that ran the exec() command 
terminates immediately after spawning the new terminal. The new terminal
must be closed manually by the user.

tldr: this file opens a new terminal, but it cannot automatically close the new
terminal instance. So the user has to manually close the new terminal window 
when they are done.
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include "terminal.h"
#include "color.h"
#include "MCP.h"


/* List of paths for supported terminals for this system. */
/* Important: Must be in the same order as enum Terminal. */
char terminals[SUPPORTED_TERMS][25] = {
    XFCE4_PATH, 
    GNOME_PATH
};


/**
 * @brief Open a new terminal to run 'top' on running processes.
 * 
 * This function attempts to open either a xfce4-terminal or a gnome-terminal
 * to run the 'top' on running processes. If xfce4-terminal or gnome-terminal
 * is not supported (i.e. not found in /usr/bin/) then this function returns
 * 0. If the MCP couldn't create a child process to run the new terminal, then
 * this function returns 0. If the child process was unable to open the
 * terminal, then this function returns -1, and the child is cleaned and
 * terminated.
 * 
 * @param[in] procqueue that contains the pids of all child processes.
 * @return 0 to indiciate MCP is returning, or -1 to indicate that a child
 * process is returning.
 */
int displayprocs (queue *procqueue)
{
    Terminal terminal;    // The type of terminal that is supported.

    if ((terminal = whichterm()) == NOTERM) {
        errorMsg("xfce4-terminal or gnome-terminal was not found in /usr/bin/");
        return 0;
    }
    if (procqueue->size > 20) {
        warningMsg("Only the first 20 PIDs will be tracked by 'top'.");
    }
    createtopscript(procqueue);
    if (openterm(terminal) == -1) {
        return -1;             // Child process will be cleaned and terminated.
    }
    return 0;
}


/**
 * @brief Find which terminal that is supported by the user.
 * 
 * This function iterates through a list of paths for supported terminals and
 * checks if the path exists on the user's machine. If so, then the code
 * representing the supported terminal is returned. Otherwise, a NOTERM code
 * is returned.
 * 
 * @return a code for the supported Terminal, or NOTERM.
 */
Terminal whichterm ()
{
    int i;    // The ith supported terminal's path.

    for (i=0; i<SUPPORTED_TERMS; i++) {
        if (access(terminals[i], F_OK) != -1) {
            return i;
        }
    }
    return NOTERM;
}


/**
 * @brief Create a bash file to execute the 'top' command on running processes.
 * 
 * This function creates a bash file that runs the command 'top -p1 -p2 ...'.
 * 
 * @param[in] procqueue that contains the pids of all child processes.
 */
void createtopscript (queue *procqueue)
{
    FILE *fp;              // File pointer to the TOPSCRIPT.
    pid_t *data;           // The data stored in a node of the procqueue.
    node *cnode = NULL;    // The current node.
    int i;          

    fp = fopen(TOPSCRIPT, "w+");
    fprintf(fp, "#!/bin/bash\n");
    fprintf(fp, "top -d0.1");
    
    i = 0;
    while ((data = (pid_t *)inorder(procqueue, &cnode)) != NULL) {
        if (i == 20)
            break;
        fprintf(fp, " -p%d", *data);
        i++;
    }
    chmod(TOPSCRIPT, S_IRWXU);
    fclose(fp);
    return;
}


/**
 * @brief Create a child process that will open a new terminal and run 'top'.
 * 
 * This function creates a new child process that will open either a 
 * xfce4-terminal or gnome-terminal and execute the TOPSCRIPT. If the MCP fails
 * to fork(), this function returns 0. If the child fails to open the terminal,
 * this function returns -1.
 * 
 * @param[in] terminal that is supported by the User.
 * @return 0 to indiciate MCP is returning, or -1 to indicate that a child
 * process is returning.
 */
int openterm (Terminal terminal)
{
    pid_t pid;    // The pid output of fork.
    /* Argument vector that has enough space to support xfce4 or gnome */
    char *argv[] = {terminals[terminal], NULL, NULL, NULL};

    if ((pid = fork()) == -1) {                            
        errorMsg("openterm: MCP unable to fork() process.");
        return 0;
    }
    if (pid > 0) {  
        waitpid(pid, NULL, 0);          // We can safely ignore waitpid errors.
        return 0;
    }
    switch (terminal) {
    case XFCE4:
        argv[1] = XFCE4_COMMAND;
        break;
    case GNOME:
        argv[1] = "--";
        argv[2] = GNOME_COMMAND;
        break;
    case NOTERM:                                      // This can never happen.
        return -1; 
    }
    if (execv(terminals[terminal], argv) == -1) {
        criticalMsg("openterm: Child unable to execv(). Child terminating.");
    }
    return -1;
}