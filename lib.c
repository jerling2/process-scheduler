#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <libgen.h>
#include "MCP.h"


queue *readfile (char *path)
{
    FILE *stream;           
    char *line;            
    size_t len;            
    ssize_t nread;
    queue *q;

    if ((stream = fopen(path, "r")) == NULL) {                    // Open file.
        fprintf(stderr, "Error cannot open %s: %s\n",
            path, strerror(errno));
        return NULL;
    }
    line = NULL;
    len = 0;
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


void addcmdline (queue *q, char *buf)
{
    cmd *tokenbuffer;
    int i;

    tokenbuffer = parseline(buf, ";");
    for (i = 0; tokenbuffer->argv[i] != NULL; i++) {
        enqueue(q, (void*) parseline(tokenbuffer->argv[i], " "));
    }
    free(tokenbuffer);
}


cmd *parseline (char *line, const char *delim)
{
    cmd *command;  // A struct for containing a tokenized array of command args.
    char *linedup;    // duplicate of the line to preserve original line.
	char *saveptr;    // strtok_r requirement.
	char *token;      // A pointer to a token in the line.
    int i;            // The ith token in the line.

	i = 0;
    linedup = strdup(line);
    command = (cmd*)malloc(sizeof(cmd));

    /* Allocate space for command argv */
    command->size = numtok(linedup, delim);
	command->argv = (char**)malloc(sizeof(char*)*command->size);

    /* Fill command argv with tokens */
	token = strtok_r(linedup, delim, &saveptr);
    command->path = strdup(token);
	while (token != NULL) {         //< strtok returns null when it's finished.
        command->argv[i] = strdup(token);
		token = strtok_r(NULL, delim, &saveptr);        //< Get the next token.
		i ++;
	}

    /* The last token in argv must be null */
	command->argv[i] = NULL;
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