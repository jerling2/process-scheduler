/*
Author: Joseph Erlinger (jerling2@uoregon.edu)
Brief :
Usuage:
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "MCP.h"


pid_t *createpool (queue *cmdqueue)
{
    cmd *command;
    pid_t *proclist;
    pid_t pid;
    int numprocs;
    int i;

    numprocs = cmdqueue->size;
    proclist = (pid_t *)malloc(sizeof(pid_t)*numprocs);
    for (i = 0; i < numprocs; i++)
    {
        command = (cmd *)dequeue(cmdqueue);
        if ((pid = fork()) == -1) {
            freecmd(command);
            return NULL;
        }
        if (pid > 0) {
            proclist[i] = pid;
            fprintf(stderr, "Created process %d : %d\n", i, pid);
            freecmd(command);
            continue;
        }
        if (execvp(command->path, command->argv) == -1) {
            perror("execvp"); // handle error
            freecmd(command);
            free(proclist);
            return NULL;
        }
    }
    fprintf(stderr, "Parent leaving create pool\n");
    return proclist;
}


int main (int argc, char *argv[]) 
{
    FILE *output_stream;    
    queue *cmdqueue;
    pid_t *proclist;


    if (argc != 2) {
        fprintf(stderr, "Error: incorrect usage. Try %s <filename>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
        
    cmdqueue = readfile(argv[1]);
    int size = cmdqueue->size;
    if ((proclist = createpool(cmdqueue)) == NULL) {
        freequeue(cmdqueue, (void *)freecmd);
        exit(EXIT_SUCCESS);
    }

    int i;
    for (i = 0; i < size; i++)
    {
        pid_t child = wait(NULL);
        printf("Child %d terminated\n", child);
    }
    
    freequeue(cmdqueue, (void *)freecmd);
    free(proclist);
    exit(EXIT_SUCCESS);
}
