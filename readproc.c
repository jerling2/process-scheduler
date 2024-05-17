#include <stdio.h>
#include <stdlib.h>
#include <string.h>


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


char state (int  pid)
{
    char state = '\0';
    extract(pid, "/proc/%d/status", "State:", "%*s %c", &state);
    return state;
}

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


int main (int argc, char *argv[])
{
    printf("Number of context switches %d\n", numctxt(953));
    printf("State is %c\n", state(953));
}