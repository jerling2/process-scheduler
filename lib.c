#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include "MCP.h"

void readfile (char *path)
{
    FILE *stream;           // Stream of the input batch file.
    char *line_buf;         // Line buffer for storing a line from the stream.
    size_t len;             // Length of the line buffer.
    ssize_t nread;          // # Bytes read from stdin.

    line_buf = NULL;
    len = 0;

    /* Open the stream */
    if ((stream = fopen(path, "r")) == NULL) {
        fprintf(stderr, "Error cannot open %s: %s\n", path, strerror(errno));
        return;                         // Error! Could not read from filename.
    }

    /* Execute each line of commands from the stream. */
    while (1) {
        if ((nread = getline(&line_buf, &len, stream)) == -1) {                              
            break;                                              // Reached EOF.
        }
        if (errno != 0) {
            fprintf(stderr, "Error reading %s: %s\n", path, strerror(errno));         
            break;                        // Encounted some error in getline().
        }
        printf("%s", line_buf);
    }

    /* Free resources and close the input stream. */
    fclose(stream);
    free(line_buf);
}   /* file_mode */