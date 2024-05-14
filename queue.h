#ifndef QUEUE_H
#define QUEUE_H

typedef struct queue {
    void *head;
    void *tail;
    int size;
} queue;

typedef struct node {
    void *next;
    void *data;
} node;

typedef void (*freefun)(void*);

void enqueue(queue *q, void *data);

void* dequeue(queue *q);

void freequeue(queue *q, freefun fun);

#endif // QUEUE_H