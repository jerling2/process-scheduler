# MCP Makefile
# Author: Joseph Erlinger (jerling2@uoregon.edu)
# Brief :
# Usuage: 

all : part1

test_queue : test_queue.o queue.o lib.o
	gcc -o test_queue -g test_queue.o queue.o lib.o

test_queue.o : Tests/test_queue.c
	gcc -o test_queue.o -c $^

part1 : part1.o lib.o queue.o
	gcc -o part1 -g part1.o lib.o queue.o

part1.o : part1.c
	gcc -o part1.o -c part1.c

lib.o : lib.c
	gcc -o lib.o -c -g -Wall lib.c

queue.o : queue.c
	gcc -o queue.o -c -g -Wall queue.c



clean :
	rm part[0-9] *.o