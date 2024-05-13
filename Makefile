# MCP Makefile
# Author: Joseph Erlinger (jerling2@uoregon.edu)
# Brief :
# Usuage: 

all : part1

part1 : part1.o lib.o
	gcc -o part1 -g part1.o lib.o

part1.o : part1.c
	gcc -o part1.o -c part1.c

lib.o : lib.c
	gcc -o lib.o -c lib.c

clean :
	rm part[0-9] *.o