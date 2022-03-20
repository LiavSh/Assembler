# Basic compilation macros
CC = gcc	# GCC Compiler
CFLAGS = -ansi -Wall -pedantic -g	# Flags
EXE_DEPS = assemblermain.o precompiler.o common.o transition1.o transition2.o	# Deps for exe

## Executable
assembler: $(EXE_DEPS)
	$(CC) -g $(EXE_DEPS) $(CFLAGS) -o $@

## Main:
assemblermain.o: assemblermain.c
	$(CC) -c assemblermain.c $(CFLAGS) -o $@

## Pre-compiler:
precompiler.o: precompiler.c precompiler.h
	$(CC) -c precompiler.c $(CFLAGS) -o $@

## Common functions:
common.o: common.c common.h
	$(CC) -c common.c $(CFLAGS) -o $@

## First transition:
transition1.o: transition1.c transition1.h 
	$(CC) -c transition1.c $(CFLAGS) -o $@

## Second transition:
transition2.o: transition2.c transition2.h 
	$(CC) -c transition2.c $(CFLAGS) -o $@

# Clean Target (remove leftovers)
clean:
	rm -rf *.o
