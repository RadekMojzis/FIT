all : test clear

CC = gcc -std=c99

test.o: test1.c
	$(CC) $^ -c

test: test.o
	$(CC) test.o -o $@

clear: 
	rm *.o
	