/*
Author: Joseph Erlinger (jerling2@uoregon.edu)
Brief :
Usuage:
*/
#include <stdio.h>
#include <stdlib.h>
#include "MCP.h"


int main(int argc, char *argv[]) {
    FILE *output_stream;
    int flags, opt;        
    char *filename;
    
    if (argc != 2) {
        fprintf(stderr, "Error: incorrect usage. Try %s <filename>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
        
    filename = argv[1];

    readfile(filename);
    exit(EXIT_SUCCESS);
}
