/*
Joseph Erlinger (jerling2@uoregon.edu)

This file contains a queue datastructure where each node within the queue
encapsulates generic data. A user interacting with the queue does not need to
interact with the node structure.
*/
#include <stdio.h>
#include <stdlib.h>
#include "MCP.h"
#include "color.h"


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

    if (q == NULL) {
        return;
    }
    while ((data = dequeue(q)) != NULL) {
        freemem(data); 
    }
    free(q);
}


/**
 * @brief Inorder-traverse a queue.
 * 
 * This function traverses a queue inorder by repeating getting the next node.
 * To start at the begining, call this function with cnode = NULL.
 * 
 * @param[in] queue Pointer to be traverse.
 * @param[in] cnode Pointer to the current node.
 */
void *inorder(queue *q, node **cnode)
{
    if (q->size == 0) {
        return NULL;
    }
    if (*cnode == NULL) {
        *cnode = q->head;
    } else if ((*cnode)->next == NULL) {
        *cnode = NULL;
        return NULL;
    } else {
        *cnode = (*cnode)->next;
    }
    return (*cnode)->data;
}


/**
 * @brief Remove a node based on its data from a queue.
 * 
 * This function traverses a queue inorder until it reaches a node that contains
 * the target data. Then, that node is removed by calling swap and free.
 * 
 * @param[in] queue containing the target data.
 * @param[in] target Pointer used to identify the node to be removed.
 */
int rmqueue(queue *q, void *target)
{
    node *cnode = NULL;    // Pointer to the current node.
    void *data;            // Pointer to the data stored in the current node.

    while ((data = inorder(q, &cnode)) != NULL) {
        if (data == target) {
            swap(q, cnode);
            free(cnode);
            return 0;
        }
    }
    return -1;
}


/**
 * @brief Remove a specific node from the queue.
 * 
 * This function makes a target node's predecessor and successor point to each
 * other instead of pointing to target node. Thereby removing the target node
 * from the queue. Note: this function does not free any memory.
 * 
 * @param[in] queue containing the removed node.
 * @param[in] node to be removed.
 */
void swap(queue *q, node *removed) {
    node *next;    // The removed node's successor.
    node *prev;    // The removed node's predecessor.

    q->size --;
    next = removed->next; 
    prev = removed->prev;
    if (q->head == removed) {
        q->head = next;
    }
    if (q->tail == removed) {
        q->tail = prev;
    }
    if (next != NULL) {
        next->prev = prev;
    }
    if (prev != NULL) {
        prev->next = next;
    }
}


/**
 * @brief Return data from the head and send the node to the back of the queue.
 * 
 * @param[in] queue containing nodes.
 * @return data of the demoted node.
 */
void *demote(queue *q)
{
    node *oldhead;    // The node to be demoted (sent to back of queue).
    node *oldtail;    // The old tail.
    node *newhead;    // The new head.

    if (q->size == 0) {
        return NULL;                      // Queue is empty. No data to return.
    }
    if (q->size == 1) {
        return q->head->data;            // Cannot demote because head == tail.
    }
    oldhead = q->head;
    oldtail = q->tail;
    newhead = oldhead->next;
    oldtail->next = oldhead;
    newhead->prev = NULL;
    oldhead->next = NULL;
    oldhead->prev = oldtail;
    q->head = newhead;
    q->tail = oldhead;
    return oldhead->data;
}