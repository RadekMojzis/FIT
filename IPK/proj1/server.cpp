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
	cout << "socket set up.. \n";
	if(welcome_socket.setup_server(6699))
		return 1;
	char str[INET6_ADDRSTRLEN];
	struct sockaddr_in6 sa_client;
	while(1){
		socklen_t sa_client_len = sizeof(sa_client);
		int comm_socket = accept(welcome_socket.number(), (struct sockaddr*)&sa_client, &sa_client_len);		
		if (comm_socket > 0)
		{
			if(inet_ntop(AF_INET6, &sa_client.sin6_addr, str, sizeof(str))) {
				cout << "INFO: New connection:\n";
				cout << "INFO: Client address is " << str << "\n";
				cout << "INFO: Client port is" << ntohs(sa_client.sin6_port) << "\n";
			}
			
			char buff[1024];
			int res = 0;
			for (;;)		
			{	
				res = recv(comm_socket, buff, 1024,0);
                if (res <= 0)                
                    break;
                                                			
			  send(comm_socket, buff, strlen(buff), 0);
			}
		}
		else
		{
			printf(".");
		}
	}

	
	return 0;
}
