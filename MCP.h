/* -- MCP Header --
Author: Joseph Erlinger (jerling2@uoregon.edu)
Brief : MCP Version 1.0
*/

#ifndef MCP_H
#define MCP_H

typedef struct cmd {
    char *path;
    char **argv;
    int size;
} cmd;

typedef struct queue {
    struct node *head;
    struct node *tail;
    int size;
} queue;

typedef struct node {
    struct node *next;
    struct node *prev;
    void *data;
} node;

typedef void (*freefun)(void*);

/* Queue Methods*/
queue *newqueue();

void enqueue(queue *q, void *data);

void *dequeue(queue *q);

void freequeue(queue *q, freefun fun);

void *inorder(queue *q, node **cnode);

void rmqueue(queue *q, void *target);

void swap(queue *q, node *removed);

void *demote(queue *q);

/* Parse Methods */
queue *readfile (char *path);

void addcmdline (queue *q, char *buf);

int numtok (char *buf, const char *delim);

cmd *parseline (char *line, const char *delim);

void freecmd (cmd *command);

#endif 