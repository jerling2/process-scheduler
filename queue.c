/*
Joseph Erlinger (jerling2@uoregon.edu)

This file contains a queue datastructure where each node within the queue
encapsulates generic data. A user interacting with the queue does not need to
interact with the node structure.
*/
#include <stdio.h>
#include <stdlib.h>
#include "MCP.h"

/** 
 * @brief Create an empty queue. 
 *
 * This function allocates memory for an empty queue structure. It returns a
 * pointer to the queue.
 * 
 * @return Pointer to the newly allocated queue.
 */
queue *newqueue()
{
    queue *q;    // The empty queue.
    
    q = (queue*)malloc(sizeof(queue));
    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
    return q;
}


/**
 * @brief Enqueue generic data to a queue.
 * 
 * This function encapsulates generic data within a node structure and enqueues
 * this node into a queue.
 * 
 * @param[in,out] queue Pointer where the node will be enqueued.
 * @param[in] data Pointer to the data to be encapsulated in the node.
 */
void enqueue(queue *q, void *data)
{
    node *tail;    // Old tail of the queue.
    node *new;     // New tail of the queue.
    
    new = (node*)malloc(sizeof(node));
    new->prev = NULL;
    new->next = NULL;
    new->data = data;
    if (q->size == 0) {                        // Edge case: the first enqueue.
        q->head = new;
        q->tail = new;
        q->size ++;
        return;
    }
    tail = q->tail;                                            // Default case.
    tail->next = new;
    new->prev = tail;
    q->tail = new;
    q->size ++;
}


/**
 * @brief Dequeue generic data from a queue.
 * 
 * This function removes the head of a queue and returns the data encapusulated
 * in this head. Then, this function updates the head Pointer (and sometimes
 * the tail Pointer) of the queue and deallocates the memory used by the old 
 * head node.
 * 
 * @param[in,out] queue Pointer where the head node will be dequeued.
 * @return Pointer to the data contained by the dequeued head node.
 */
void *dequeue(queue *q)
{
    node *head;       // Old head of the queue.
    node *new;        // New head of the queue.
    void *data;       // Data of the dequeued node.
   
    if (q->size == 0) {                              // Edge case: empty queue.
        return NULL;
    }
    head = q->head;
    data = head->data;
    if (q->size == 1) {                             // Edge case: size 1 queue.
        q->head = NULL;
        q->tail = NULL;
        q->size --;
        free(head);
        return data;
    }
    new = head->next;                                          // Default case.
    new->prev = NULL;
    q->head = new;
    q->size --;
    free(head);
    return data;
}


/**
 * @brief Deallocate memory reserved by a queue.
 * 
 * This function deallocates all memory of reserved by a queue including the
 * data Pointer of each node, each node structure, and the queue structure.
 * 
 * @param[in] queue Pointer to be deallocated.
 * @param[in] freefun Pointer where the freefun is a procedure thats deallocate
 * the data Pointer of a node.
 */
void freequeue(queue *q, freefun freemem)
{
    void *data;    // Data of a dequeued node.

    while ((data = dequeue(q)) != NULL) {
        freemem(data); 
    }
    free(q);
}