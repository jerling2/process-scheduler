#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "terminal.h"

char terminals[SUPPORTED_TERMS][25] = {
    XFCE4, 
    GNOME
};

Terminal whichterm ()
{
    int i;

    for (i=0; i<SUPPORTED_TERMS; i++) {
        if (access(terminals[i], F_OK) != -1) {
            return i;
        }
    }
    return -1;
}