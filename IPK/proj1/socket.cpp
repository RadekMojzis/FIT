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
Socket::~Socket(){
	shutdown (socket_number, 2);
}

Socket::Socket(){
	if((socket_number = socket(AF_INET6, SOCK_STREAM, 0)) < 0){
    cerr << "Sorry, couldnt initialise socket!\n";
  }
}

bool Socket::setup(std::string server_hostname, int port_number){
	struct hostent *server;

	if ((server = gethostbyname(server_hostname.c_str())) == NULL) {
	 cerr << "ERROR, no such host\n";
	 return true;
	}
	
	bzero((char *) &server_address, sizeof(server_address));
	server_address.sin6_family = AF_INET6;
	bcopy((char *)server->h_addr, (char *)&server_address.sin6_addr.s6_addr, server->h_length);
	server_address.sin6_port = htons(port_number);
	
  cout << "im just gonna connect here real quick :/ \n";
	
	if (connect(socket_number, (const struct sockaddr *) &server_address, sizeof(server_address)) != 0){
		cerr << "could not connect to host :x\n";
	  return true;        
  }
  cout << "done\n";
  return false;
}

bool Socket::setup_server(int port_number){
	
	int optval = 1;
	setsockopt(socket_number, SOL_SOCKET, SO_REUSEADDR,(const void *)&optval , sizeof(int)); 

	memset(&server_address, 0 ,sizeof(server_address));
	server_address.sin6_family = AF_INET6;
	server_address.sin6_addr = in6addr_any;
	server_address.sin6_port = htons(port_number); 

	if (bind(socket_number, (struct sockaddr *) &server_address , sizeof(server_address)) < 0){
	  cerr << "ERROR: bind\n";
	  return true;
	}
	if ((listen(socket_number, 1)) < 0){
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