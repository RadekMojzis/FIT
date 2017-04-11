#include <iostream>
#include "socket.h"
#include "packet.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

class Socket{
  private:
    int socket_number;
    int domain;
		int type;
		struct sockaddr_in server_address;
		struct sockaddr_in6 server_address_in6;
		struct sockaddr *address;
    
		
  public:
    bool init(int domain, int type);
    bool connect(string address, int port);
    bool listen(port);
    bool send(Packet packet);
    bool receave(Packet packet);
};
    
bool init(int domain, int type){
  if(domain == IPv4) domain = AF_INET;
	if(domain == IPv6) domain = AF_INET6;
	if(type == UDP) type = SOCK_DGRAM;
	if(type == TCP) type = SOCK_STREAM;
	
	if((socket_number = socket(domain, type, 0)) < 0){
    cerr << "Sorry, couldnt initialise socket!\n";
    return true;
  }
  return false;
}

bool Socket::connect(string server_hostname, int port_number){
	struct hostent *server;

	if ((server = gethostbyname(server_hostname)) == NULL) {
	 cerr << "ERROR, no such host\n";
	 return true;
	}
	
	bzero((char *) &server_address, sizeof(server_address));
	server_address.sin_family = domain;
	bcopy((char *)server->h_addr, (char *)&server_address.sin_addr.s_addr, server->h_length);
	server_address.sin_port = htons(port_number);
  
	if (connect(socket_number, (const struct sockaddr *) &server_address, sizeof(server_address)) != 0){
		cerr << "could not connect to host :x\n";
	  return true;        
  }
  
  return false;
}

bool Socket::setup(int port_number){
	
	int optval = 1;
	setsockopt(socket_number, SOL_SOCKET, SO_REUSEADDR,(const void *)&optval , sizeof(int)); 
	
	if(domain == IPv4){
		bzero((char *) &server_address, sizeof(server_address));
		server_address.sin_family = AF_INET;
		server_address.sin_addr.s_addr = htonl(INADDR_ANY);
		server_address.sin_port = htons((unsigned short)port_number);
		address = (struct sockaddr *) &server_address;
  }
	if(domain == IPv6){
		memset(&server_address_in6, 0 ,sizeof(server_address_in6));
 	  server_address_in6.sin6_family = AF_INET6;
	  server_address_in6.sin6_addr = in6addr_any;
	  server_address_in6.sin6_port = htons(port_number); 
		address = (struct sockaddr *) &server_address_in6;
	}
	
	if (bind(socket_number, address , sizeof(*address)) < 0){
	  cerr << "ERROR: bind\n";
	  return true;
	}
	
  return false;
}

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
