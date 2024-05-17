/*
Author: Joseph Erlinger (jerling2@uoregon.edu)
*/
#ifndef READPROC_H
#define READPROC_H

// Scan some bit of information from a file.
void extract(int pid, char *filepath, char *match, char *pattern, void *data);

// Get the state information of a process.
char state (int  pid);

// Get the information about the total number of context-switches of a process.
int numctxt (int  pid);

// Get the total ram allocation information of a process.
char *ram (int pid);

// Get the IO bytes_read information of a process.
char *readBytes (int pid);

// Get the IO bytes_write information of a process.
char *writeBytes (int pid);

// Print the header of the table.
void printheader();

// Print a row of the table.
void printrow(int pid);

#endif 