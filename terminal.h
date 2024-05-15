#pragma once

typedef enum Terminal {
    XFCE4,
    GNOME,
    NOTERM = -1
} Terminal;

#define XFCE4 "/usr/bin/xfce4-terminal"
#define GNOME "/usr/bin/gnome-terminal"
#define SUPPORTED_TERMS 2

extern char terminals[SUPPORTED_TERMS][25];



Terminal whichterm ();

void displayprocs (Terminal terminal);