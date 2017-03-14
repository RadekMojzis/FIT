#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "socket.hpp"
#include "packet.hpp"  
#include <string>
#include <stdio.h>
#include <iostream>
#include <errno.h>
#include <cstdlib>

using namespace std;

int get_args(int argc, char *argv[], string *root_folder, int *port){
	int option;
  while((option = getopt(argc, argv, "r:p:")) != -1){
		switch(option){
			case 'r':
			  *root_folder = optarg;
			  break;
			case 'p':
				*port = atoi(optarg);
				/*if(errno){
					cerr << "Port out of range of int... seriously?!";
				}*/
			  break;
		  default:
			  cout << "Usage: ftrestd -r <ROOT-FOLDER> -p <PORT>";
				return 0;
			  break;
		}
	}
}

int main (int argc, char* argv[]){
	string root_folder = "/";
	int port = 6677;
	get_args(argc, argv, &root_folder, &port);

	
	Socket welcome_socket;
  if(welcome_socket.init()) return 1;
	
	if(welcome_socket.listen(port)) return 1;
  
  cout << "server set, waiting for connection..\n";
  
	Socket comm_socket;
  while(1){
    if(comm_socket.Accept(welcome_socket.number()))return 1;
    if(comm_socket.number() > 0){
			Packet pack;
			Packet& packet = pack;
      if(!comm_socket.recv(pack)){
        cout << "Packet receaved, Analysing:" << packet.get_str() << "\n";
				if(comm_socket.send(pack)) return 1;
				if(comm_socket.send(pack)) return 1;
			}
	  }
	  comm_socket.Close();
  }
  
	return 0;
}
