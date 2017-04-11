all : test clear

CC = gcc -std=c++1y -Wno-write-strings 

test.o: test.cpp
	$(CC) $^ -c

test: test.o
	$(CC) test.o -o $@

clear: 
	rm *.o
	