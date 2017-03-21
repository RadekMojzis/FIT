all : ftrest ftrestd clear

CC = g++ -std=c++1y -Wno-write-strings 

client.o: client.cpp
	$(CC) $^ -c
server.o: server.cpp
	$(CC) $^ -c
packet.o: packet.cpp
	$(CC) $^ -c
socket.o: socket.cpp
	$(CC) $^ -c

ftrest: client.o packet.o socket.o
	$(CC) client.o packet.o socket.o -o $@
  
ftrestd: server.o packet.o socket.o
	$(CC) server.o packet.o socket.o -o $@

clear: 
	rm *.o
	