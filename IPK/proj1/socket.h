
#ifndef MY_SOCKET_DEFINITION
#define MY_SOCKET_DEFINITION

#include "packet.h"

enum ip_protocol{IPv4, IPv6}

class Socket{
  private:
    int socket_number;
    int protocol;
    
  public:
    bool init(ip_protocol type);
    bool connect(string address, int port);
    bool listen(port);
    bool send(Packet packet);
    bool receave(Packet packet);
};

#endif