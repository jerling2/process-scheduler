#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "terminal.h"


char terminals[SUPPORTED_TERMS][25] = {
    XFCE4, 
    GNOME
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
    char *argv[] = {terminals[type], "--command=top -p1", NULL};
    execv(terminals[type], argv);
}

void createtopscript (pid_t *proclist, int numprocs)
{
    FILE *fp;
    int i;

    fp = fopen(TOPSCRIPT, "w+");
    fprintf(fp, "top");
    for (i=0; i<numprocs; i++) {
        fprintf(fp, " -p%d", proclist[i]);
    }
    fclose(fp);
}