#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "socket.hpp"
#include "packet.hpp"
#include <string>
#include <iostream>
#include <cstdlib>
#include <time.h>
#include <algorithm>
#include <cerrno>
#include <cstdio>
#include <cstring>
using namespace std;

void help(){
	cout << "Usage: ipk-scanner -i <interface> -f <file>\n";
}

int main (int argc, const char * argv[]) {
  if(argc != 5){help(); return 0;}
	string inter, f;
	string &interface = inter;
	string &file = f;

	get_args(argc, argv, interface, file);

	
	int broadcastSock = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
	struct sockaddr_in s;

	if(broadcastSock < 0)
			return;

	memset(&s, '\0', sizeof(struct sockaddr_in));
	s.sin_family = AF_INET;
	s.sin_port = (in_port_t)htons(tcpSocket ? tcpSocket : 3310);
	s.sin_addr.s_addr = htonl(INADDR_BROADCAST);

	char mess[500] = "ahoj vole!";
	
	if(sendto(broadcastSock, mess, strlen(mess), 0, (struct sockaddr *)&s, sizeof(struct sockaddr_in)) < 0)
			perror("sendto");
	
	
	

  return 0;
}
