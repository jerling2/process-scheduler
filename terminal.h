/**
 * Joseph Erlinger (jerling2@uoregon.edu)
 * YouTube Tutorial: "Using ANSI escape codes" by AtoZ Programming Tutorials.
 * Special Thanks: Paul Silisteanu for the helpful YouTube tutorial.
*/
#pragma once
#include <stdio.h>


enum Colors {
    RESET_COLOR,
    YELLOW = 33,
    CYAN = 36,
};


static inline void setColor (int code)
{
    printf("\033[%d;1m", code);
}


static inline void resetColor ()
{
    printf("\033[0m");
}


static inline void createMsg (int pid)
{
    setColor(CYAN);
    printf("Child created pid=%d\n", pid);
    resetColor();
    fflush(stdout);
}


static inline void terminateMsg (int pid)
{
    setColor(YELLOW);
    printf("Child terminated pid=%d\n", pid);
    resetColor();
    fflush(stdout);
}