/*
Joseph Erlinger (jerling2@uoregon.edu)

This file loads a file of commands into a queue, then delegates each command to
its own subprocess.
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "MCP.h"
#include "terminal.h"


/**
 * @brief Launch a pool of subprocesses to handle commands.
 * 
 * This function creates a n-processes given a queue of size n. The parent
 * process (aka the MCP) manages the queue so that each child process gets one
 * command. Each child process attempts to execute their assigned command. If
 * the child fails, then they release all of their resources and call exit.
 * 
 * @param[in,out] queue Pointer that contains n-commands.
 * @param[in] integer equal to n for n-processes.
 * @return Pointer to an array of child pids.
 */
pid_t *createpool (queue *cmdqueue, int numprocs)
{
    cmd *command;       // The command that will be executed by a subprocess.
    pid_t *proclist;    // List of child pids.
    pid_t pid;          // The pid output of fork.
    int i;              // The ith process.

    if (cmdqueue->size != numprocs) {          // Validate function parameters.
        fprintf(stderr, "Queue size and numprocs are not equal.\n");
        return NULL;
    }

    proclist = (pid_t *)malloc(sizeof(pid_t)*numprocs);
    for (i = 0; i < numprocs; i++) {
        command = (cmd *)dequeue(cmdqueue);
        if (command->path == NULL) {       // Handle poor constructed commands.
            freecmd(command);
            continue;
        }
        if ((pid = fork()) == -1) {                             // Fork process.
            perror("fork");
            goto error;                                    // Handle fork error.
        }
        if (pid > 0) {                                             // MCP Logic.
            proclist[i] = pid;
            freecmd(command);
            createMsg(pid);
            continue;
        }
        if (execvp(command->path, command->argv) == -1) {  // Subprocess logic.
            perror("execvp");
            goto error;
        }
    }
    return proclist;

    error:
    freecmd(command);
    free(proclist);
    return NULL;
}


/**
 * @usuage ./part1 <filename>
 */
int main (int argc, char *argv[]) 
{
    queue *cmdqueue = NULL;     // Queue of commands.
    pid_t *proclist = NULL;     // List of child pids.
    int numprocs;               // Number of child pids.
    int i;                      // The ith subprocess.

    if (argc != 2) {                                       // Input validation.
        fprintf(stderr, "Error: invalid usage. Try %s <filename>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    if ((cmdqueue = readfile(argv[1])) == NULL) {
        goto cleanup;                            // Error: Could not read file.
    }
    numprocs = cmdqueue->size;
    if ((proclist = createpool(cmdqueue, numprocs)) == NULL) {
        goto cleanup;              // Error (most likely from a child process).
    }
    for (i = 0; i < numprocs; i++) {
        pid_t child = wait(NULL);     // Wait for each child process to finish.
        terminateMsg(child);
    }

    // TODO: integrate
    // const char *gnome_command = "gnome-terminal -- 'top'";
    // int ret = system(gnome_command);
    // printf("%d\n", ret);

    cleanup:
    freequeue(cmdqueue, (void *)freecmd);
    free(proclist);
    exit(EXIT_SUCCESS);
}
