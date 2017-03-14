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

void help(){
	cout << "Usage: ftrest COMMAND REMOTE-PATH [LOCAL-PATH]\n";
}

int main (int argc, const char * argv[]) {
  if(argc < 3 || argc > 4){help();return 0;}
	string action = argv[1];
	string path = argv[2];
	switch(action){
		case "del":
		  cout << "smaže soubor určený REMOTE-PATH na serveru";
		  break;
		case "get":
			cout << "get zkopíruje soubor z REMOTE-PATH do aktuálního lokálního adresáře či na místo určené pomocí LOCAL-PATH je-li uvedeno";
		  break;
		case "put":
			cout << "put zkopíruje soubor z LOCAL-PATH do adresáře REMOTE-PATH ";
		  break;
		case "lst":
			cout << "ls";
		  break;
		case "mkd":
			cout << "smaže soubor určený REMOTE-PATH na serveru";
		  break;
		case "rmd":
			cout << "smaže soubor určený REMOTE-PATH na serveru";
		  break;
		default:
		  help();
			return 0;
	}
	del smaže soubor určený REMOTE-PATH na serveru



mkd vytvoří adresář specifikovaný v REMOTE-PATH na serveru
rmd odstraní adresář specifikovaný V REMOTE-PATH ze serveru
	
  Socket comm_socket;

	Packet packet;
	Packet& message = packet;

  if(comm_socket.init()                    ) return 1;
	if(comm_socket.connect("localhost", 6677)) return 1;
	
	message.set_str("Echo receaved!");

	comm_socket.send(message);
	comm_socket.recv(message);

	cout << message.get_str() << "\n";
	
  comm_socket.Close();
  return 0;
}
