all : test clear

CC = gcc -std=gnu99

test.o: test.c
	$(CC) $^ -c

test: test.o
	$(CC) test.o -o $@

clear: 
	rm *.o
	