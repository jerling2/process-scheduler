# MCP Makefile
# Author: Joseph Erlinger (jerling2@uoregon.edu)

CFLAGS = -c -g -Wall
CC = gcc
src_dir = src
objects = parser.o queue.o terminal.o readproc.o
targets = part1 part2 part3 part4

project : $(targets)

all : project cpubound iobound

part1 : $(src_dir)/part1.o $(objects:%=$(src_dir)/%)
	$(CC) -o $@ $^

part2 : $(src_dir)/part2.o $(objects:%=$(src_dir)/%)
	$(CC) -o $@ $^

part3 : $(src_dir)/part3.o $(objects:%=$(src_dir)/%)
	$(CC) -o $@ $^

part4 : $(src_dir)/part4.o $(objects:%=$(src_dir)/%)
	$(CC) -o $@ $^

cpubound : $(src_dir)/cpubound.o
	$(CC) -o $@ $^

iobound : $(src_dir)/iobound.o
	$(CC) -o $@ $^

$(src_dir)/%.o: $(src_dir)/%.c
	$(CC) $(CFLAGS) -o $@ -c $<

clean :
	rm part[0-9] $(src_dir)/*.o

cleanall : clean
	rm iobound cpubound