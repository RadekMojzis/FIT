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
Socket::~Socket(){
	shutdown (socket_number, 2);
}

bool Socket::init(){
	if((socket_number = socket(AF_INET, SOCK_STREAM, 0)) < 0){
    cerr << "Sorry, couldnt initialise socket!\n";
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

bool Socket::Accept(Socket welcome_socket){
  socket_number = 0;
  //cout << "frog 2\n";
  //socklen_t socklen = sizeof(server_address);
  socket_number = accept(welcome_socket.number(), NULL, NULL);
  cout << "welcome socket number" << welcome_socket.number() << "\n";
  cout << "comm socket number" << socket_number << "\n";
  //cout << "frog 3\n";
  /*if (socket_number > 0){
    char str[INET_ADDRSTRLEN];
    if(inet_ntop(AF_INET, &server_address.sin_addr, str, sizeof(str))) {
      cout << "INFO: New connection:\n";
      cout << "INFO: Client address is " << str << "\n";
      cout << "INFO: Client port is" << ntohs(server_address.sin_port) << "\n";
    }
  }*/
  return false;
}

bool Socket::connect(std::string server_hostname, int port_number){
	struct hostent *server;

	if ((server = gethostbyname(server_hostname.c_str())) == NULL) {
	 cerr << "ERROR, no such host\n";
	 return true;
	}
	cout << "getting server adress..\n";
  
	bzero((char *) &server_address, sizeof(server_address));
	server_address.sin_family = AF_INET;
	bcopy((char *)server->h_addr, (char *)&server_address.sin_addr.s_addr, server->h_length);
	server_address.sin_port = htons(port_number);
  
	printf("INFO: Server socket: %s : %d \n", inet_ntoa(server_address.sin_addr), ntohs(server_address.sin_port));

	if (::connect(socket_number, (const struct sockaddr *) &server_address, sizeof(server_address)) != 0){
		cerr << "could not connect to host :x\n";
	  return true;
  }
  cout << "done\n";
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
/*
bool Socket::send(Packet packet){
  if((sendto(client_socket, packet.get_str(), strlen(buf), 0, (struct sockaddr *) &server_address, sizeof(server_address))) < 0){
		cerr << "Couldnt send packet :/\n";
		return true;
  }
  return false;
}

bool Socket::receave(Packet packet){
  if (recvfrom(client_socket, buf, BUFSIZE, 0,(struct sockaddr *) &server_address, &serverlen) < 0){
		cerr << "Failed to recieve shit :{\n";
		return true;
	}
 
	return false;
}
*/