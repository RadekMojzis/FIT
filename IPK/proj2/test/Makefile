all : test test1 test2 test3 clear

CC = gcc -std=gnu99

test.o: test.c
	$(CC) $^ -c

test1.o: test1.c
	$(CC) $^ -c

test2.o: test2.c
	$(CC) $^ -c

test3.o: test3.c
	$(CC) $^ -c

test: test.o
	$(CC) test.o -o $@

test1: test1.o
	$(CC) test1.o -o $@
	
test2: test2.o
	$(CC) test2.o -o $@

test3: test3.o
	$(CC) test3.o -o $@

clear: 
	rm *.o
	