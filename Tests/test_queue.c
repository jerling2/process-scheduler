#include <stdio.h>
#include <stdlib.h>
#include "../MCP.h"

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
    cmd *three;

    /* Allocate memory for queue and two commands */
    q = (queue*)malloc(sizeof(queue));
    one = (cmd*)malloc(sizeof(cmd));
    two = (cmd*)malloc(sizeof(cmd));
    three = (cmd*)malloc(sizeof(cmd));

    /* Populate command one & two */
    one->size = 1;
    one->argv = (char**)malloc(sizeof(char*)*one->size);
    one->argv[0] = NULL;
    one->path = NULL;
    two->size = 2;
    two->argv = (char**)malloc(sizeof(char*)*two->size);
    two->argv[0] = (char*)malloc(sizeof(char)*5);
    two->argv[1] = NULL;
    two->path = NULL;
    three->size = 3;
    three->argv = (char**)malloc(sizeof(char*)*three->size);
    three->argv[0] = (char*)malloc(sizeof(char)*5);
    three->argv[1] = NULL;
    three->argv[2] = NULL;
    three->path = NULL;

    /* Initialize Queue */
    q->head = NULL;
    q->tail = NULL;
    q->size = 0;

    /* Enqueue Test */
    printf("Enqueue Test\n");
    printqueue(q);
    enqueue(q, one);
    printqueue(q);
    enqueue(q, two);
    printqueue(q);
    enqueue(q, three);
    printqueue(q);
    printf("\n");
    /* Inorder Test*/
    node *start = NULL;
    cmd *data;
    printf("Inorder traversal:\n{ ");
    while ((data = (cmd *)inorder(q, &start)) != NULL) {
        printf("%d ", data->size);
    }
    printf("}\n\n");

    printf("Remove second element:\n");
    rmqueue(q, two);
    printqueue(q);

    printf("Remove first element:\n");
    rmqueue(q, one);
    printqueue(q);

    printf("Remove last element:\n");
    rmqueue(q, three);
    printqueue(q);
    printf("\n");

    enqueue(q, one);
    enqueue(q, two);
    enqueue(q, three);
    
    /* Dequeue Test */
    printf("Dequeue Test\n");
    printqueue(q);
    while (dequeue(q) != NULL) {
        printqueue(q);
    }

    /* Freequeue Test */
    enqueue(q, one);
    enqueue(q, two);
    enqueue(q, three);
    freequeue(q, (void*) freecmd);
}

