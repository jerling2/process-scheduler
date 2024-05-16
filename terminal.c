#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include "terminal.h"
#include "color.h"


char terminals[SUPPORTED_TERMS][25] = {
    XFCE4_PATH, 
    GNOME_PATH
};


int displayprocs (pid_t *proclist, int numprocs)
{
    Terminal code;
    if ((code = whichterm()) == -1) {
        errorMsg("xfce4-terminal or gnome-terminal was not found in /usr/bin/");
        return 0;
    }
    createtopscript(proclist, numprocs);
    if (openterm(code) == -1) {
        return -1;
    }
    removetopscript();
    return 0;
}


Terminal whichterm ()
{
    int i;

    for (i=0; i<SUPPORTED_TERMS; i++) {
        if (access(terminals[i], F_OK) != -1) {
            return i;
        }
    }
    return NOTERM;
}


/* Possible error if createtopscript cannot open TOPSCRIPT. */
void createtopscript (pid_t *proclist, int numprocs)
{
    FILE *fp;
    int i;

    fp = fopen(TOPSCRIPT, "w+");
    fprintf(fp, "top -d0.1");
    for (i=0; i<numprocs; i++) {
        fprintf(fp, " -p%d", proclist[i]);
    }
    chmod(TOPSCRIPT, S_IRWXU);
    fclose(fp);
    return;
}


int openterm (Terminal type)
{
    pid_t pid;
    char *argv[] = {terminals[type], NULL, NULL, NULL};

    if ((pid = fork()) == -1) {                            
        errorMsg("openterm: MCP unable to fork() process.");
        return 0;
    }
    if (pid > 0) {  
        waitpid(pid, NULL, 0); // We can safely ignore waitpid errors.
        return 0;
    }
    switch (type) {
    case XFCE4:
        argv[1] = XFCE4_COMMAND;
        break;
    case GNOME:
        argv[1] = "--";
        argv[2] = GNOME_COMMAND;
        break;
    case NOTERM:
        exit(EXIT_SUCCESS); // Terminate child.
    }
    if (execv(terminals[type], argv) == -1) {
        criticalMsg("openterm: Child unable to execv(). Child terminating.");
    }
    return -1;
}


void removetopscript()
{
    if (unlink(TOPSCRIPT) == -1) 
        perror("removetopscript");
    return;
}