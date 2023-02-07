# Makefile for 'rsabasic.c'
#
# Fangzhou Yu
# CS 62, Winter 2023
 
CC=gcc
OBJS = rsabasic.o
MAKE = make
CC = gcc

desW23: rsabasic.o
	gcc rsabasic.o -o rsabasic -lm

desW23.o: rsabasic.c
	gcc -c rsabasic.c -o rsabasic.o -lm

test: 
	./rsabasic

# clean up after our compilation
clean:
	rm -f core
	rm -f *~ *.o
	rm -f rsabasic
