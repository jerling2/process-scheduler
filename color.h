/**
 * Joseph Erlinger (jerling2@uoregon.edu)
 * YouTube Tutorial: "Using ANSI escape codes" by AtoZ Programming Tutorials.
 * Special Thanks: Paul Silisteanu for the helpful YouTube tutorial.
*/
#pragma once
#include <stdio.h>

enum Colors {
    RESET_COLOR,
    RED = 31,
    YELLOW = 33,
    MAGENTA = 35,
    CYAN = 36
};

static inline void setBoldColor (int code)
{
    printf("\033[%d;1m", code);
}

static inline void setColor (int code)
{
    printf("\033[%dm", code);
}

static inline void resetColor ()
{
    printf("\033[0m");
}

/**
 * @brief Print in bold cyan text, "Child Created pid= ...".
 */
static inline void createMsg (int pid)
{
    setBoldColor(CYAN);
    printf("Child created pid=%d\n", pid);
    resetColor();
    fflush(stdout);
}

/**
 * @brief Print in bold yellow text, "Child terminated pid= ...".
 */
static inline void terminateMsg (int pid)
{
    setBoldColor(YELLOW);
    printf("Child terminated pid=%d\n", pid);
    resetColor();
    fflush(stdout);
}

/**
 * @brief Print in bold red text, "Error: ...".
 */
static inline void errorMsg (char *message)
{
    setBoldColor(RED);
    printf("Error: %s\n", message);
    resetColor();
    fflush(stdout);
}

/**
 * @brief Print in bold red text, "Critical: ...".
 */
static inline void criticalMsg (char *message)
{
    setBoldColor(RED);
    printf("Critical: %s\n", message);
    resetColor();
    fflush(stdout);
}

/**
 * @brief Print in bold magenta text, "Info: ...".
 */
static inline void infoMsg (char *message)
{
    setBoldColor(MAGENTA);
    printf("Info: %s\n", message);
    resetColor();
    fflush(stdout);
}

/**
 * @brief Print in bold magenta text, "Info: %s%d".
 */
static inline void infoNumMsg (char *message, int num)
{
    setBoldColor(MAGENTA);
    printf("Info: %s%d\n", message, num);
    resetColor();
    fflush(stdout);
}