# MCP Makefile
# Author: Joseph Erlinger (jerling2@uoregon.edu)
# Brief :
# Usuage: 

all : part1

part1 : part1.o
	gcc -o part1 -g part1.o

part1.o : part1.c
	gcc -o part1.o -c part1.c

clean :
	rm part[0-9] part[0-9].o