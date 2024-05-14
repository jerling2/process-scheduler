#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include "MCP.h"


void enqueue(queue *q, void *data)
{
    node *tail;
    node *new;

    /* Initialize new node */
    new = (node*)malloc(sizeof(node));
    new->prev = NULL;
    new->next = NULL;
    new->data = data;

    /* Edge case: the first enqueue */
    if (q->size == 0) {
        q->head = new;
        q->tail = new;
        q->size ++;
        return;
    }
    
    /* Otherwise, default enqueue */
    tail = q->tail;
    tail->next = new;
    new->prev = tail;
    q->tail = new;
    q->size ++;
}


void *dequeue(queue *q)
{
    node *head;
    node *new;
    void *data;
    
    /* Edge case: empty queue */
    if (q->size == 0) {
        return NULL;
    }

    /* Grab the head*/
    head = q->head;
    data = head->data;

    /* Edge case: size 1 queue */
    if (q->size == 1) {
        q->head = NULL;
        q->tail = NULL;
        q->size --;
        free(head);
        return data;
    }

    /* Otherwise, default dequeue */
    new = head->next;
    new->prev = NULL;
    q->head = new;
    q->size --;
    free(head);
    return data;
}


void freequeue(queue *q, freefun fun)
{
    void *n;
    while ((n = dequeue(q)) != NULL) {
        fun(n);
    }
    free(q);
}