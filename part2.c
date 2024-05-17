/*
Joseph Erlinger (jerling2@uoregon.edu)

This file loads a file of commands into a queue, then delegates each command to
its own subprocess.
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <signal.h>
#include "MCP.h"
#include "color.h"
#include "terminal.h"

void alarmhandler(int num) {
    write(1, "Alarm recieved!\n", 16);
    alarm(1);
    return;
}

void handler(int num) {
    write(1, "User signal recieved!\n", 22);
    kill(getpid(), SIGUSR1);
    return;
}

/**
 * @brief Launch a pool of subprocesses to handle commands.
 * 
 * This function creates a n-processes given a queue of size n. The parent
 * process (aka the MCP) manages the queue so that each child process gets one
 * command. Each child process attempts to execute their assigned command. If
 * the child fails, then they release all of their resources and call exit.
 * 
 * @param[in,out] cmdqueue that contains n-commands. Note: this queue will be
 * consumed/emptied by this function.
 * @return procqueue that contains k-processes.
 */
queue *createpool (queue *cmdqueue, sigset_t sigset)
{
    queue *procqueue;    // A queue to store processes in.
    cmd *command;        // The command that will be executed by a subprocess.
    pid_t *pid;          // The pid output of fork.
    int numcmds;         // The size of the cmdqueue.
    int i;               // The ith command.
    int sig;

    procqueue = newqueue();
    numcmds = cmdqueue->size;
    for (i = 0; i < numcmds; i++) {
        pid = (pid_t *)malloc(sizeof(pid_t));
        command = (cmd *)dequeue(cmdqueue);
        if (command->path == NULL) {       // Handle poor constructed commands.
            freecmd(command);
            continue;
        }
        if ((*pid = fork()) == -1) {                            // Fork process.
            criticalMsg("createpool: MCP unable to fork() process.");
            continue;                         // We don't want the MCP to exit.
        }
        if (*pid > 0) {                                            // MCP Logic.
            enqueue(procqueue, pid);
            freecmd(command);
            createMsg(*pid);
            continue;
        }
        sigwait(&sigset, &sig);
        if (execvp(command->path, command->argv) == -1) {       // Child logic.
            fprintf(stderr, "Could not execute '%s'. ", command->path);
            perror("execvp");
            freecmd(command);
            free(pid);
            freequeue(procqueue, free);
            return NULL;
        }
    }
    return procqueue;
}


/**
 * @usuage ./part1 <filename>
 */
int main (int argc, char *argv[]) 
{
    queue *cmdqueue = NULL;     // Queue of commands.
    queue *procqueue = NULL;    // Queue of running processes.
    int numchild;               // Number of children.
    int i;                      // The ith child.
    pid_t mcppid;               // Process ID of the Master Control Program.
    pid_t *pid;                 // A child's pid.
    node *cnode = NULL;         // Node representing the currrent node.
    sigset_t sigset;            // Set of signals 
    int sig;
    struct sigaction sa_usr1;
    struct sigaction sa_alrm;
   
    /* Don't allow SIGUSR1 to kill */
    sigemptyset(&sigset); 
    sigaddset(&sigset, SIGUSR1);
    sigaddset(&sigset, SIGALRM);
    sigaddset(&sigset, SIGSTOP);
    sigprocmask(SIG_BLOCK, &sigset, NULL);  // Prevents weird segfaults.
    
    sa_usr1.sa_handler = handler;
    sa_alrm.sa_handler = alarmhandler;

    sigaction(SIGUSR1, &sa_usr1, NULL);
    sigaction(SIGALRM, &sa_alrm, NULL);
    sigaction(SIGSTOP, &sa_usr1, NULL);

    if (argc != 2) {                                       // Input validation.
        fprintf(stderr, "Error: invalid usage. Try %s <filename>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    if ((cmdqueue = readfile(argv[1])) == NULL) {
        goto cleanup;                            // Error: Could not read file.
    }
    if ((procqueue = createpool(cmdqueue, sigset)) == NULL) {
        goto cleanup;      // Child process needs to be cleaned and terminated.
    }
    if (displayprocs(procqueue) == -1) {
        goto cleanup;      // Child process needs to be cleaned and terminated.
    }
    
    infoMsg("MCP is waiting for x seconds.");
    alarm(5);
    sigwait(&sigset, &sig);
    mcppid = getpid();
    printf("\033[35;1mMCP pid=%d\n\033[0m", mcppid);
    while ((pid = (pid_t *)inorder(procqueue, &cnode)) != NULL) {
        setpgid(*pid, mcppid);
    }
    
    infoMsg("MCP is sending SIGUSR1 to all its children.");
    kill(0, SIGUSR1);
    infoMsg("MCP is sending SIGSTOP to all its children.");
    
    while ((pid = (pid_t *)inorder(procqueue, &cnode)) != NULL) {
        kill(*pid, SIGSTOP);
    }
    
    while ((pid = (pid_t *)inorder(procqueue, &cnode)) != NULL) {
        kill(*pid, SIGCONT);
    }
    
    infoMsg("MCP is waiting.");
    numchild = procqueue->size;
    for (i = 0; i < numchild; i++) {
        pid_t child = wait(NULL);     // Wait for each child process to finish.
        terminateMsg(child);
    }

    cleanup:
    freequeue(cmdqueue, (void *)freecmd);
    freequeue(procqueue, free);
    exit(EXIT_SUCCESS);
}
