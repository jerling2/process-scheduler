#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "terminal.h"


char terminals[SUPPORTED_TERMS][25] = {
    XFCE4_PATH, 
    GNOME_PATH,
};


Terminal whichterm ()
{
    int i;

    for (i=0; i<SUPPORTED_TERMS; i++) {
        if (access(terminals[i], F_OK) != -1) {
            return i;
        }
    }
    return -1;
}


void displayprocs (Terminal type)
{
    pid_t pid;
    char *argv[] = {terminals[type], NULL, NULL, NULL};
    if ((pid = fork()) == -1) {                            
        perror("fork");
        return;
    }
    if (pid > 0) {                                            
        return;
    }
    switch (type) {
    case XFCE4:
        argv[1] = XFCE4_COMMAND;
        break;
    case GNOME:
        argv[1] = "--";
        argv[2] = GNOME_COMMAND;
        break; 
    }
    execv(terminals[type], argv);
}

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
}