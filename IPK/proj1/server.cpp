#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "socket.hpp"
#include "packet.hpp"  
#include <string>
#include <stdio.h>
#include <string.h>
#include <iostream>
using namespace std;
int main (int argc, const char * argv[]) {
	cout << "starting\n";
	Socket welcome_socket;
  welcome_socket.init();
	cout << "socket set up.. \n";
	if(welcome_socket.listen(6666))
		return 1;
  
  cout << "server set, waiting for connection..\n";
	char str[INET_ADDRSTRLEN];
	struct sockaddr_in sa_client;
  Socket comm_socket;
  while(1){
    cout << "welcome socket: " << welcome_socket.number() << "\n";
    comm_socket.Accept(welcome_socket);
    
    /*socklen_t sa_client_len = sizeof(sa_client);
    
    int comm_socket = accept(welcome_socket.number(), (struct sockaddr*)&sa_client, &sa_client_len);		
	  
    if (comm_socket > 0){
			if(inet_ntop(AF_INET, &sa_client.sin_addr, str, sizeof(str))) {
				cout << "INFO: New connection:\n";
				cout << "INFO: Client address is " << str << "\n";
				cout << "INFO: Client port is" << ntohs(sa_client.sin_port) << "\n";
	    }*/
    if(comm_socket.number() > 0){
      char buff[1024];
      int res = 0;
      for (;;){	
        //res = recv(comm_socket, buff, 1024,0);
        res = recv(comm_socket.number(), buff, 1024,0);
        if (res <= 0)                
          break;
                                                      
        //send(comm_socket, buff, strlen(buff), 0);
        send(comm_socket.number(), buff, strlen(buff), 0);
      }
	  }
	  else{
     printf(".");
    }
    comm_socket.Close();
    //close(comm_socket);
  }

	
	return 0;
}
