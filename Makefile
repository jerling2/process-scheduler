# MCP Makefile
# Author: Joseph Erlinger (jerling2@uoregon.edu)
# Brief :
# Usuage: 

all : part1

test_queue : test_queue.o queue.o parser.o
	gcc -o test_queue -g test_queue.o queue.o parser.o

test_queue.o : Tests/test_queue.c
	gcc -o test_queue.o -c -g $^

part1 : part1.o parser.o queue.o
	gcc -o part1 -g part1.o parser.o queue.o

part1.o : part1.c
	gcc -o part1.o -c -g part1.c

parser.o : parser.c
	gcc -o parser.o -c -g -Wall parser.c

queue.o : queue.c
	gcc -o queue.o -c -g -Wall queue.c



clean :
	rm part[0-9] *.o