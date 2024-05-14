#include <stdio.h>
#include <stdlib.h>
#include "../MCP.h"
#include "../queue.h"

void printqueue(queue *q)
{
    /* Try to get the head */
    node *n; 
    n = q->head;
    
    /* Edge case: empty queue */
    if (n == NULL) {
        printf("[]\n");
        return;
    }

    /* Default case */
    printf("[");
    printf("%d", ((cmd*)n->data)->size);
    n = n->next;
    while (n != NULL) {
        printf(", %d", ((cmd*)n->data)->size);
        n = n->next;
    }
    printf("]\n");
}

int main (int argc, char *argv[]) 
{
    queue *q;
    cmd *one;
    cmd *two;

    /* Allocate memory for queue and two commands */
    q = (queue*)malloc(sizeof(queue));
    one = (cmd*)malloc(sizeof(cmd));
    two = (cmd*)malloc(sizeof(cmd));

    /* Populate command one & two */
    one->size = 1;
    one->argv = (char**)malloc(sizeof(char*)*one->size);
    one->argv[0] = NULL;
    two->size = 2;
    two->argv = (char**)malloc(sizeof(char*)*two->size);
    two->argv[0] = (char*)malloc(sizeof(char)*5);
    two->argv[1] = NULL;

    /* Initialize Queue */
    q->head = NULL;
    q->tail = NULL;
    q->size = 0;

    /* Enqueue Test */
    printqueue(q);
    enqueue(q, one);
    printqueue(q);
    enqueue(q, two);
    printqueue(q);
    
    /* Dequeue Test */
    while (dequeue(q) != NULL) {
        printqueue(q);
    }

    /* Freequeue Test */
    enqueue(q, one);
    enqueue(q, two);
    freequeue(q, (void*) freecmd);
}

