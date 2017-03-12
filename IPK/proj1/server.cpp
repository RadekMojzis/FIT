#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
   
int main (int argc, const char * argv[]) {
  
  int welcome_socket;
  if((welcome_socket = socket(PF_INET6, SOCK_STREAM, 0)) < 0){
    cerr << "Failed to initialise welcome socket, sorry mate :(";
  }
  sockaddr_in welcome_socket_adress;
  
  if(bind(welcome_socket, , 0))
}
