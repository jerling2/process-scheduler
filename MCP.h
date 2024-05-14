/* -- MCP Header --
Author: Joseph Erlinger (jerling2@uoregon.edu)
Brief : MCP Version 1.0
*/

#ifndef MCP_H
#define MCP_H

typedef struct cmd {
    const char *path;
    char *const argv;
} cmd;

void readfile (char *path);

#endif