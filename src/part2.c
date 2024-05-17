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


/**
 * @brief Resend any missed signals for safety.
 * 
 * This function never gets called, and I'm unsure if its necessary.
 * 
 * @param[in] signum the number representing the type of signal received. 
 */
void handler (int signum)
{
    kill(getpid(), signum);
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
queue *createpool (queue *cmdqueue)
{
    queue *procqueue;    // A queue to store processes in.
    cmd *command;        // The command that will be executed by a subprocess.
    pid_t *pid;          // The pid output of fork.
    int numcmds;         // The size of the cmdqueue.
    int i;               // The ith command.
    sigset_t sigset;     // Sigset containing SIGUSR1.
    int sig;             // Signal number (used by sigwait).

    sigemptyset(&sigset);
    sigaddset(&sigset, SIGUSR1);

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
 * @usuage ./part2 <filename>
 */
int main (int argc, char *argv[]) 
{
    queue *cmdqueue = NULL;     // Queue of commands.
    queue *procqueue = NULL;    // Queue of running processes.
    int numchild;               // Number of children.
    int i;                      // The ith child.
    pid_t mcppid;               // Process ID of the MCP.
    pid_t *pid;                 // A child's pid.
    node *cnode = NULL;         // Node representing the currrent node.
    struct sigaction sa;        // sigaction to handle ansychronous signals.
    sigset_t sigset;            // Set of custom handled signals.
    int sig;                    // Signal number (used by sigwait) 

    if (argc != 2) {                                       // Input validation.
        fprintf(stderr, "Error: invalid usage. Try %s <filename>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    /* Initialize sigaction struct */
    sa.sa_handler = handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    /* Initialize and add signals to sigset */
    sigemptyset(&sigset);
    sigaddset(&sigset, SIGUSR1);
    sigaddset(&sigset, SIGALRM);

    /* Block SIGUSR1 and SIGALRM making them behave synchronously */
    sigprocmask(SIG_BLOCK, &sigset, NULL);

    /* Attach signals to the handler function */
    sigaction(SIGUSR1, &sa, NULL);
    sigaction(SIGALRM, &sa, NULL);

    /* Get PID of MCP to be later used to set the PGID */
    mcppid = getpid();
    infoNumMsg("MCP pid=", mcppid);

    if ((cmdqueue = readfile(argv[1])) == NULL) {
        goto cleanup;                            // Error: Could not read file.
    }
    if ((procqueue = createpool(cmdqueue)) == NULL) {
        goto cleanup;      // Child process needs to be cleaned and terminated.
    }
    if (displayprocs(procqueue) == -1) {
        goto cleanup;      // Child process needs to be cleaned and terminated.
    }

    while ((pid = (pid_t *)inorder(procqueue, &cnode)) != NULL) {
        setpgid(*pid, mcppid);           // Put all children in the same Group.
    }

    /* ------------------------------------------- */
    /*         Subprocesses are not running        */
    /* ------------------------------------------- */

    /* Wait before allowing subprocesses to exec() */
    infoMsg("MCP is waiting for alarm (in 5 seconds)");
    infoMsg("Subprocesses will be allowed to run for 2 seconds.");
    alarm(5);
    sigwait(&sigset, &sig);                // Consume the pending alarm signal.
    infoMsg("MCP sent SIGUSR1 to children.");
    killpg(mcppid, SIGUSR1);
    sigwait(&sigset, &sig);              // Consume the pending SIGUSR1 signal.
    
    /* ------------------------------------------- */
    /*           Subprocesses are running          */
    /* ------------------------------------------- */

    /* Stop subprocesses in two seconds. */
    infoMsg("MCP will stop subprocesses in 2 seconds");
    alarm(2);
    sigwait(&sigset, &sig);              // Consume the pending SIGUSR1 signal.
    while ((pid = (pid_t *)inorder(procqueue, &cnode)) != NULL) {
        kill(*pid, SIGSTOP);                      // Signal STOP to each child.
    }

    /* ------------------------------------------- */
    /*         Subprocesses are not running        */
    /* ------------------------------------------- */

    /* Wait five seconds before allowing subprocesses continue */
    infoMsg("MCP is waiting for alarm (in 5 seconds)");
    infoMsg("Subprocesses will be allowed to finish.");
    alarm(5);
    sigwait(&sigset, &sig);                // Consume the pending alarm signal.
    infoMsg("MCP sent SIGCONT to children.");
    while ((pid = (pid_t *)inorder(procqueue, &cnode)) != NULL) {
        kill(*pid, SIGCONT);                      // Signal CONT to each child.
    }
    
    /* ------------------------------------------- */
    /*           Subprocesses are running          */
    /* ------------------------------------------- */

    numchild = procqueue->size;
    for (i = 0; i < numchild; i++) {
        pid_t child = wait(NULL);     // Wait for each child process to finish.
        terminateMsg(child);
    }

    /* Free all resources before leaving. */
    cleanup:
    freequeue(cmdqueue, (void *)freecmd);
    freequeue(procqueue, free);
    exit(EXIT_SUCCESS);
}