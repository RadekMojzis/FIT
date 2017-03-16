#include <iostream>
#include "socket.hpp"
#include "packet.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <netdb.h>
#include <strings.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>

Socket::~Socket(){
	shutdown (socket_number, 2);
}

bool Socket::init(){
	if((socket_number = socket(AF_INET, SOCK_STREAM, 0)) < 0){
    cerr << "Sorry, couldnt initialise socket!\n";
    return true;
	}
  return false;
}

void Socket::Close(){
  ::close(socket_number);
  return;
}

int Socket::number(){
  return socket_number;
}

bool Socket::Accept(int welcome_socket){
  socket_number = accept(welcome_socket, NULL, NULL);
	if(socket_number < 0){
		cout << "Accept error! code:" << strerror(errno) << "\n";
    return true;
	}
	return false;
}

bool Socket::connect(std::string server_hostname, int port_number){
	struct hostent *server;

	if ((server = gethostbyname(server_hostname.c_str())) == NULL) {
	 cerr << "ERROR, no such host\n";
	 return true;
	}
  
	memset((char *) &server_address, 0, sizeof(server_address));
	server_address.sin_family = AF_INET;
	memcpy((char *)&server_address.sin_addr.s_addr, (char *)server->h_addr, server->h_length);
	server_address.sin_port = htons(port_number);

	if (::connect(socket_number, (const struct sockaddr *) &server_address, sizeof(server_address)) != 0){
		cerr << "could not connect to host :x\n";
	  return true;
  }
  return false;
}

bool Socket::listen(int port_number){
	int optval = 1;
	setsockopt(socket_number, SOL_SOCKET, SO_REUSEADDR,(const void *)&optval , sizeof(int)); 

	memset(&server_address, 0 ,sizeof(server_address));
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = htonl(INADDR_ANY); 
	server_address.sin_port = htons(port_number); 

	if (bind(socket_number, (struct sockaddr *) &server_address , sizeof(server_address)) < 0){
	  cerr << "ERROR: bind\n";
	  return true;
	}
	if ((::listen(socket_number, 1)) < 0){
    cerr << "ERROR: listen\n";
    return true;
  }
  return false;
}

bool Socket::send(Packet packet){
	if(::send(socket_number, packet.get_str(), packet.length(), 0) <= 0){
		cerr << "Couldnt send packet :/\n";
		return true;
  }
  return false;
}

bool Socket::recv(Packet &packet){
	int rcv;
	char buffer[1024];
	packet.set_str("");
  if((rcv = ::recv(socket_number, buffer, 1024, 0))){
		if(rcv == -1){
			cerr << "Unable to receave packet!\n";
			return true;
		}
		packet.append(buffer);
		memset(buffer, 0, sizeof(buffer));
	}
	return false;
}
