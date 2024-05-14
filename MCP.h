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

cmd *readfile (char *path);

int numtok (char *buf, const char *delim);

cmd parseline (char *line, const char *delim);

void freecmd (cmd *command);

#endif 