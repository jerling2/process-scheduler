/*
Joseph Erlinger (jerling2@uoregon.edu)

This file scans the /proc directory to display relevant data about processes.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Scan some bit of information from a file.
void extract(int pid, char *filepath, char *match, char *pattern, void *data)
{
    FILE *fp;
    char *line;
    char *path; 
    path = (char *)malloc(sizeof(char)*BUFSIZ);
    line = (char *)malloc(sizeof(char)*BUFSIZ);
    sprintf(path, filepath, pid);
    if ((fp = fopen(path, "r")) == NULL) {
        free(line);
        free(path);
        return;
    }
    while (fgets(line, BUFSIZ, fp)) {
        if (strstr(line, match) != NULL) {
            sscanf(line, pattern, data);
            fclose(fp);
            free(line);
            free(path);
            return;
        }
    }
    fclose(fp);
    free(line);
    free(path);
}

// Get the state information of a process.
char state (int  pid)
{
    char state = '\0';
    extract(pid, "/proc/%d/status", "State:", "%*s %c", &state);
    return state;
}

// Get the information about the total number of context-switches of a process.
int numctxt (int  pid)
{
    int ctxt = 0;
    int temp = 0;
    extract(pid, "/proc/%d/status", "voluntary_ctxt", "%*s%d", &temp);
    ctxt += temp;
    extract(pid, "/proc/%d/status", "nonvoluntary_ctxt", "%*s%d", &temp);
    ctxt += temp;
    return ctxt;
}

// Get the total ram allocation information of a process.
char *ram (int pid)
{
    int rss = -1;
    int ram;
    int pagesize;
    char *ram_str;
    pagesize = getpagesize();
    extract(pid, "/proc/%d/statm", "", "%*d%d", &rss);
    ram = (rss * pagesize) / 1024;
    ram_str = (char *)malloc(sizeof(char)*BUFSIZ);
    sprintf(ram_str, "%dKB", ram);
    return ram_str;
}

// Get the IO bytes_read information of a process.
char *readBytes (int pid)
{
    int num = -1;
    char *bytes;

    bytes = (char *)malloc(sizeof(char)*BUFSIZ);
    extract(pid, "/proc/%d/io", "read_bytes", "%*s%d", &num);
    num /= 1024;
    sprintf(bytes, "%d KB", num);
    return bytes;
}

// Get the IO bytes_write information of a process.
char *writeBytes (int pid)
{
    int num = -1;
    char *bytes;

    bytes = (char *)malloc(sizeof(char)*BUFSIZ);
    extract(pid, "/proc/%d/io", "write_bytes", "%*s%d", &num);
    num /= 1024;
    sprintf(bytes, "%dKB", num);
    return bytes;
}

// Print the header of the table.
void printheader()
{
    printf("\x1b[2;30;47m%16s %16s %16s %16s %16s %16s\x1b[0m\n",
    "PID", "STATE", "IO READ", "IO WRITE", "MEM IN RAM", "CTXT-SWITCHES");
}

// Print a row of the table.
void printrow(int pid)
{
    char *rbytes = readBytes(pid);
    char *wbytes = writeBytes(pid);
    char *mem = ram(pid);

    printf("%16d %16c %16s %16s %16s %16d\n", 
    pid, state(pid), rbytes, wbytes, mem, numctxt(pid));
    free(rbytes); free(wbytes); free(mem);
}