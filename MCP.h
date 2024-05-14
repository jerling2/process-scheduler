/* -- MCP Header --
Author: Joseph Erlinger (jerling2@uoregon.edu)
Brief : MCP Version 1.0
*/

#ifndef MCP_H
#define MCP_H

typedef struct cmd {
    const char *path;
    char *const argv;
    int size;
} cmd;

void readfile (char *path);

cmd parseline (char *line);

void freecmd (cmd *command);

#endif