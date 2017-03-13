#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "socket.hpp"
#include "packet.hpp"
#include <string>
#include <iostream>

using namespace std;
	
int main (int argc, const char * argv[]) {
  cout << "starting \n";
	Socket comm_socket;
  comm_socket.init();
	cout << "socket set up.. \n";
	if(comm_socket.connect("localhost", 6666)){
    return 1;
  }
    
	cout << "Connected! \n";
	char buf[] = "Echo receaved!\n";
  cout << "sending a message! \n";
	
  send(comm_socket.number(), buf, 1024, 0);
	recv(comm_socket.number(), buf, 1024, 0);
	cout << buf << "\n";
	
  comm_socket.Close();
  
  return 0;
}
