/*
Author: Joseph Erlinger (jerling2@uoregon.edu)
Brief :
Usuage:
*/
#include <stdio.h>
#include <stdlib.h>
#include "MCP.h"


int main (int argc, char *argv[]) 
{
    FILE *output_stream;
    int flags, opt;        
    char *filename;
    queue *q;
    node *n;
    
    if (argc != 2) {
        fprintf(stderr, "Error: incorrect usage. Try %s <filename>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
        
    filename = argv[1];
    q = readfile(filename);
    
    /* Testing readfile */
    n = q->head;
    int i = 0;
    int k;
    while (n != NULL) {
        printf("Node %d:\n", i);
        cmd temp = *(cmd*)n->data;
        printf("\tpath=\"%s\"\n\targv={%s", temp.path, temp.argv[0]);
        for (k = 1; ((cmd*)n->data)->argv[k] != NULL; k++) {
            printf(", %s", ((cmd*)n->data)->argv[k]);
        }
        printf("}\n");
        n = n->next;
        i++;
    }
    
    cmd *temp = (cmd *)dequeue(q);
    printf("Dequeued Node: path=%s, size=%d\n", temp->path, temp->size);
    freecmd(temp);

    freequeue(q, (void *)freecmd);
    exit(EXIT_SUCCESS);
}
