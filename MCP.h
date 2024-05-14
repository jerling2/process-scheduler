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

queue *newqueue();

void enqueue(queue *q, void *data);

void *dequeue(queue *q);

void freequeue(queue *q, freefun fun);

queue *readfile (char *path);

int numtok (char *buf, const char *delim);

cmd *parseline (char *line, const char *delim);

void freecmd (cmd *command);

#endif 