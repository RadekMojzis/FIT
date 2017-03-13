
#ifndef MY_SOCKET_DEFINITION
#define MY_SOCKET_DEFINITION

#include "packet.h"

const int IPv4 = 0;
const int IPv6 = 1;

const int TCP = 0;
const int UDP = 1;

class Socket{
  private:
    int socket_number;
    int domain;
		int type;
		struct sockaddr *address;
		
  public:
    bool init(int domain, int type);
    bool connect(string address, int port_number);
    bool setup(port_number);
    bool send(Packet packet);
    bool receave(Packet packet);
};

#endif