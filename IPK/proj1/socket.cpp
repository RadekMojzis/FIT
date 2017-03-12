#include <iostream>
#include "socket.h"
#include "packet.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

private:
    int socket_number;
    int protocol;
    
  public:
    bool init();
    bool connect(string address, int port);
    bool send(Packet packet);
    bool receave(Packet packet);
    
bool Socket::init(ip_protocol type){
  if(type == IPv4){
    socket_number = socket(AF_INET, SOCK_STREAM, 0);
    protocol = type;
  }
  if(type == IPv6){
    socket_number = socket(AF_INET6, SOCK_STREAM, 0);
    protocol = type;
  }
  if(socket_number < 0){
    cerr << "Sorry, couldnt initialise socket!\n";
    return true;
  }
  return false;
}

bool Socket::connect(string ip, int port){
  sockaddr *final_adress;
  
  if(protocol == IPv4){
    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    const char *cstr = ip.c_str();
    inet_pton(cstr, &address.sin_addr.s_addr);

    final_adress = (sockaddr*)&address;
  }
  
  if(protocol == IPv6){
    sockaddr_in6 address;
    address.sin_family = AF_INET6;
    address.sin_port = htons(port);
    const char *cstr = ip.c_str();
    inet_pton(cstr, &address.sin_addr.s_addr);

    final_adress = (sockaddr*)&address;
  }
  
  if(bind(socket_number, final_adress, sizeof(*final_adress)) < 0){
    return true;
  }
  
  return false;
}


bool Socket::listen(int port){
  sockaddr *final_adress;
  
  if(protocol == IPv4){
    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr = inaddr_any
    
    final_adress = (sockaddr*)&address;
  }
  
  if(protocol == IPv6){
    sockaddr_in6 address;
    address.sin_family = AF_INET6;
    address.sin_port = htons(port);
    address.sin_addr = in6addr_any;
    
    final_adress = (sockaddr*)&address;
  }
  
  if(bind(socket_number, final_adress, sizeof(*final_adress)) < 0){
    cerr << "Couldnt bind socket adress :(!\n";
    return true;
  }
  if(listen(socket_number, 1) < 0){
    cerr << "Ooops i cant hear anything :(!\n";
    return true;
  }
  
  return false;
}

bool Socket::send(Packet packet){
  return false;
}
bool Socket::receave(Packet packet){
  return false;
}
