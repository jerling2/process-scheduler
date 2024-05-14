#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <libgen.h>
#include "MCP.h"


/**
 * @brief Create a queue that contains all the commands in a file.
 *
 * This function reads commands from a file stream. Each line of a file stream
 * can contain multiple commands. Each command is encapuslated in a cmd
 * structure and enqueued to a result queue.
 * 
 * @param[in] path Pointer to a filename.
 * @return queue Pointer to a queue containing cmd structures, or Null if
 * the path could not be opened.
 */
queue *readfile (char *path)
{
    FILE *stream;         // The file Stream that this function reads from.
    char *line = NULL;    // The line read from a file stream.
    size_t len = 0;       // (Use internally by getline)
    ssize_t nread;        // Number of Bytes read from a file stream.
    queue *q;             // Pointer to the resulting queue.

    if ((stream = fopen(path, "r")) == NULL) {                    // Open file.
        fprintf(stderr, "Error cannot open %s: %s\n", path, strerror(errno));
        return NULL;
    }
    q = newqueue();
    while ((nread = getline(&line, &len, stream)) != -1) {        // Read file.
        addcmdline(q, line);                                      
    }
    if (errno != 0) {              
        fprintf(stderr, "Error when reading %s: %s\n", path, strerror(errno));
    }
    fclose(stream);                                       // Release resources.
    free(line);
    return q;
}   /* readfile */


/**
 * @brief Enqueue commands from a line buffer into the input queue.
 * 
 * This function splits a line buffer into large tokens, then creates cmd
 * structures from each large token, and enqueues each cmd structure into the 
 * input queue.
 * 
 * @param[in,out] queue Pointer where the cmds structures are enqueued to.
 * @param[in] buf Pointer to a line buffer containing commands.
 */
void addcmdline (queue *q, char *buf)
{
    cmd *tokenbuffer;    // Temporary buffer for storing whole commands.
    cmd *command;        // Command structure that is enqueued to the queue.
    int i;               // The ith command in the tokenbuffer.

    tokenbuffer = parseline(buf, ";");
    for (i = 0; tokenbuffer->argv[i] != NULL; i++) {
        command = parseline(tokenbuffer->argv[i], " ");
        enqueue(q, (void*) command);
    }
    free(tokenbuffer);
}

/**
 * @brief Create a cmd structure from a input line buffer.
 * 
 * This function uses strtok_r to create a cmd structure that contains tokens
 * from an input line buffer. The input line buffer will not be altered.
 * 
 * @param[in] line Pointer to a buffer.
 * @param[in] delimiter to split the line buffer into tokens.
 * @return cmd structure containing tokens parsed from the line buffer.
*/
cmd *parseline (char *line, const char *delim)
{
    cmd *command;     // Cmd structure to contain the processed input line.
    char *linedup;    // Duplicate string to preserve the original line.
	char *saveptr;    // (Use internally by strtok_r)
	char *token;      // Pointer to a token returned by strtok_r.
    int i;            // The ith token in the line.

	i = 0;
    linedup = strdup(line);
    /* Initialize the command structure */
    command = (cmd*)malloc(sizeof(cmd));
    command->size = numtok(linedup, delim);
	command->argv = (char**)malloc(sizeof(char*)*command->size);
    /* Extract tokens from the line */
    token = strtok_r(linedup, delim, &saveptr);         // Get the first token.
    command->path = strdup(token);
	while (token != NULL) {     
        command->argv[i] = strdup(token);
		token = strtok_r(NULL, delim, &saveptr);         // Get the next token.
		i ++;
	}
	command->argv[i] = NULL;             // Last argument in argv must be null.
    free(linedup);
    return command;
}   /* parseline */


int numtok (char *buf, const char *delim)
{
    int tok;      // Number of tokens.
	int state;    // The current state of the algorithm.
	int i;        // The ith char in the buf.

	tok = 1;                   //< Start at 1 to make space for the null token.
	state = 0;
	i = 0;
    
    /* Strip newline character (if any) */
    strtok (buf, "\n"); 
    
    /* Skip all leading delimiters */
    while (buf[i] == delim[0]) {
		i ++;
    }	
    
    /* Increment tokens only when moving from state 0 to 1 */
	for (; buf[i] != '\0'; i++) {
        /* State 0: Skip all delimiters */
		if (state == 0 && buf[i] != delim[0]) {
			state = 1;
			tok ++;                                       //< Increment tokens.
		}
        /* State 1: Skip all non-delimiters */
		if (state == 1 && buf[i] == delim[0]) {
			state = 0;
        }
	}
    return tok;
}   /* numtok */


void freecmd (cmd *command)
{
    int i;    // The ith argument in command's argv.

    i = 0;

    for (; i < command->size; i++) {
        free(command->argv[i]);
    }
    free(command->path);
    free(command->argv);
    free(command);
}   /* freecmd */