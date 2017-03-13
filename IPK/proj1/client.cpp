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
  cout << "socket set up.. \n";
	Socket comm_socket;
	cout << "socket set up.. \n";
	comm_socket.setup("localhost", 6699);
	cout << "socket set up.. \n";
	char buf[] = "Ahoj vole!\n";
	
	while(1){
		send(comm_socket.number(), buf, 1024, 0);
		recv(comm_socket.number(), buf, 1024, 0);
		cout << buf << "\n";
	}
	return 0;
}
