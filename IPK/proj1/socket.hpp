
#ifndef MY_SOCKET_DEFINITION
#define MY_SOCKET_DEFINITION

#include "packet.hpp"
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>


const int IPv4 = 0;
const int IPv6 = 1;

const int TCP = 0;
const int UDP = 1;
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
    bool Accept(Socket welcome_socket);
    void Close();
    //bool send(Packet packet);
    //bool receave(Packet packet);
};



#endif