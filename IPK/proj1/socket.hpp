
#ifndef MY_SOCKET_DEFINITION
#define MY_SOCKET_DEFINITION

#include "packet.hpp"
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>

using namespace std;
class Socket{
  private:
    int socket_number;
    struct sockaddr_in server_address;
		
  public:
		~Socket();
		int number();
    bool connect(string address, int port);
    bool listen(int port);
    bool init();
    bool Accept(int welcome_socket);
    void Close();
    bool send(class Packet packet);
    bool recv(class Packet &packet);
};

#endif