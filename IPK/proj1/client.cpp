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
using namespace std;

void help(){
	cout << "Usage: ftrest COMMAND REMOTE-PATH [LOCAL-PATH]\n";
}

int parse_input(string input, string& hostname, int& port, string& remote_path){
	if(input.find("http://") != 0){
		cerr << "couldnt find 'http://'";
	  return 1;
	}
	string rest_of_input = input.substr (7); // here magic happens << no actually 7 i the length of 'http://'
	//cout << rest_of_input << "\n";
	hostname = rest_of_input.substr(0, rest_of_input.find(":"));
	//cout << "hostname: " << hostname << "\n";
	rest_of_input.erase(0, rest_of_input.find(":")+1);
	//cout << rest_of_input << "\n";
	port = strtol(rest_of_input.c_str(), 0, 10);
	if(errno == ERANGE){
		cerr << "Port error: Number out of range....\n";
		return 1;
	}
	//cout << "port number: " << port << "\n";
	rest_of_input.erase(0, rest_of_input.find("/"));
	//cout << rest_of_input << "\n";
	remote_path = rest_of_input;
	//cout<< "remote path: " << remote_path << "\n";
	return 0;
}

int main (int argc, const char * argv[]) {
  if(argc < 3 || argc > 4){help(); return 0;}
	FILE* put_target = NULL;
	
	string action = argv[1];
	int    p; 
	string rem_pat;
	string host;
	
	int&    port        = p;
	string& remote_path = rem_pat;
	string& hostname    = host;
	
	string local_path = "/";
	
	if(parse_input(argv[2], hostname, port, remote_path)){return 1;}
	
	transform(action.begin(), action.end(), action.begin(), ::tolower);
	
	if(action == "get")
		if(argc == 4) 
			local_path = argv[3];
	if(action == "put"){
		if(argc != 4){
			help(); return 1;
		}
		local_path = argv[3];
	}
	
	if(action != "lst" && action != "mkd" && action != "rmd" && action != "put" && action != "get" && action != "del"){
		cout << action << "\n";
	  help();
		return 1;
	}
	
	Packet packet;
	Packet& message = packet;
	int filesize = 0;
	if(action == "put"){
		packet.set_str("PUT ");
		packet.append(remote_path.c_str());
		packet.append("?type=file HTTP/1.1\r\n");
		put_target = fopen(argv[3], "rb");
		if(put_target == NULL){
			cerr << "No souch file :/\n";
			return 1;
		}
		fseek (put_target , 0 , SEEK_END);
		filesize = ftell (put_target);
    rewind (put_target);
	}
	if(action == "get"){
		packet.set_str("GET ");
		packet.append(remote_path.c_str());
		packet.append("?type=file HTTP/1.1\r\n");
	}
	if(action == "del"){
		packet.set_str("DELETE ");
		packet.append(remote_path.c_str());
		packet.append("?type=file HTTP/1.1\r\n");
	}
	if(action == "mkd"){
		packet.set_str("PUT ");
		packet.append(remote_path.c_str());
		packet.append("?type=folder HTTP/1.1\r\n");
	}
	if(action == "lst"){
		packet.set_str("GET ");
		packet.append(remote_path.c_str());
		packet.append("?type=folder HTTP/1.1\r\n");
	}
	if(action == "rmd"){
		packet.set_str("DELETE ");
		packet.append(remote_path.c_str());
		packet.append("?type=folder HTTP/1.1\r\n");
	}
	
	
	time_t now = time(NULL);
	char buf[128];
  struct tm tm = *gmtime(&now);
  strftime(buf, 128, "Date: %a, %d %b %Y %H:%M:%S %Z\r\n", &tm);
	packet.append(buf);
	packet.append("Accept: application/json\r\n");
	packet.append("Accept-Encoding: identity");
	if(action == "put"){
		packet.append("\r\nContent-Type: application/octet-stream\r\n");
		packet.append("Content-Length: ");
		string filesize_str = to_string(filesize);
		packet.append(filesize_str.c_str());
	}
	packet.append("\r\n\r\n");
	if(action == "put"){
		packet.append(put_target);
	  fclose(put_target);
	}
	
	
	cout << packet.get_str();
	Socket comm_socket;

  if(comm_socket.init()                 ) return 1;
	if(comm_socket.connect(hostname, port)) return 1;

	comm_socket.send(message);
	comm_socket.recv(message);
	
	int i = 0;
  char *packet_str = message.get_str();
	string response;
	while(packet_str[i] != '\r' || packet_str[i+1] != '\n' || packet_str[i+2] != '\r' || packet_str[i+3] != '\n'){
		response += packet_str[i];
		i++;
	}
	i+=4;
		
	if(action == "get"){
		local_path+="soubor";
	  FILE *file = fopen(local_path.c_str(), "wb");
		fwrite(packet_str + i, sizeof(char), get_filesize(packet_str), file);
	}
	cout << response << "\n";
	
  comm_socket.Close();
  return 0;
}
