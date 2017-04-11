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
	cout << "Usage: ftrest COMMAND REMOTE-PATH [LOCAL-PATH]\n";
}

// parses the input string, gets hostname, port and remote path i use it as parse_input(argv[2] ... )

int parse_input(string input, string& hostname, int& port, string& remote_path){
	if(input.find("http://") != 0){
		cerr << "couldnt find 'http://'";
	  return 1;
	}
	string rest_of_input = input.substr (7); 
	hostname = rest_of_input.substr(0, rest_of_input.find(":"));
	rest_of_input.erase(0, rest_of_input.find(":")+1);
	port = strtol(rest_of_input.c_str(), 0, 10);
	if(errno == ERANGE){
		cerr << "Port error: Number out of range....\n";
		return 1;
	}
	rest_of_input.erase(0, rest_of_input.find("/"));
	remote_path = rest_of_input;
	return 0;
}

int main (int argc, const char * argv[]) {
  if(argc < 3 || argc > 4){help(); return 0;}
	
	// -----------------------  parsing of arguments -----------------------------//
	
	FILE* put_target = NULL;
	string action = argv[1];
	int    p; 
	string rem_pat;
	string host;
	
	int&    port        = p;
	string& remote_path = rem_pat;
	string& hostname    = host;
	
	string local_path = "";
	
	if(parse_input(argv[2], hostname, port, remote_path)){return 1;}
	
	transform(action.begin(), action.end(), action.begin(), ::tolower);
	
	if(action == "get"){
		if(argc == 3){
			const char *rempat = remote_path.c_str();
			local_path = rempat + remote_path.find_last_of("/")+1;
		}
		if(argc == 4)
			local_path = argv[3];
	}
	if(action == "put"){
		if(argc != 4){
			help(); return 1;
		}
		local_path = argv[3];
		if(remote_path.back() == '/'){
			if(local_path.find_last_of("/") == string::npos){
			  remote_path += local_path;
			}
			else{
				const char *locpt = local_path.c_str();
				remote_path += locpt + local_path.find_last_of("/");
			}
		}
	}
	
	if(action != "lst" && action != "mkd" && action != "rmd" && action != "put" && action != "get" && action != "del"){
		cout << action << "\n";
	  help();
		return 1;
	}
	
	// ------------------------- Creating a communication packet --------------------------
	
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
	
	// ------------------ Now the packet is ready, connect to server ----------//
	
	Socket comm_socket;

  if(comm_socket.init()                 ) return 1;
	if(comm_socket.connect(hostname, port)) return 1;

  // -------------------- Send the packet --------------------------//	
	
	if(comm_socket.send(message)){
		cerr << "Fatal error :/ nothing can be done about this...";
		comm_socket.Close();
		return 1;
	}
	// ----------------------wait for a reply -----------------------//
	if(comm_socket.recv(message)){
		cerr << "server is sending me some kind of giberish :/ i cant understand a word from it\n Go check, there might be something wrong with it";
		comm_socket.Close();
		return 1;
	}
	// --------------------- interpret the response -----------------//
	int i = 0;
  char *packet_str = message.get_str();
	string response;
	while(packet_str[i] != '\r' || packet_str[i+1] != '\n' || packet_str[i+2] != '\r' || packet_str[i+3] != '\n'){
		response += packet_str[i];
		i++;
	}
	i+=4;
  
	if(action == "get"){                                // -------------------------------------- GET
		if(response.find("200 OK\r\n") == 0){
			//cout << local_path;
			FILE *file = fopen(local_path.c_str(), "wb");
			if(file == NULL){
				cerr <<  "Unable to open file";
				return 1;
			}
			fwrite(packet_str + i, sizeof(char), get_filesize(packet_str), file);
			fclose(file);
		}
		else if(response.find("400 Bad Request\r\n") == 0 ||response.find("404 Not found\r\n") == 0){
			cerr << packet_str + i;
		}
		else{
			cerr << "unknown response from server\n"<< packet.get_str();
		}
	}else if(action == "lst"){                          // -------------------------------------- LST
		if(response.find("200 OK\r\n") == 0){
			cout << packet_str + i << "\n";
		}else if(response.find("400 Bad Request\r\n") == 0 || response.find("404 Not found\r\n") == 0){
		  cerr << packet_str + i;
		}
		else{
			cerr << "unknown response from server\n"<< packet.get_str();
		}
    
  }else if(action == "put"){                          // --------------------------------------- PUT
		if(response.find("200 OK\r\n") == 0){;}
		else if(response.find("400 Bad Request\r\n") == 0 || response.find("404 Not found\r\n") == 0){
			cerr << packet_str + i;             
		}
		else{
			cerr << "unknown response from server:\n"<< packet.get_str();
		}
	}else if(action == "mkd"){                         // ---------------------------------------- MKD
		if(response.find("200 OK\r\n") == 0){;}
		else if(response.find("400 Bad Request\r\n") == 0 || response.find("404 Not found\r\n") == 0){
			cerr << packet_str + i;                     
		}
		else{
			cerr << "unknown response from server\n"<< packet.get_str();
		}
	}else if(action == "del"){
		if(response.find("200 OK\r\n") == 0){;}        // ---------------------------------------- DEL
		else if(response.find("400 Bad Request\r\n") == 0 || response.find("404 Not found\r\n") == 0){
			cerr << packet_str + i;
		}
		else{
			cerr << "unknown response from server\n"<< packet.get_str();
		}
		
	}else if(action == "rmd"){
		if(response.find("200 OK\r\n") == 0){;}        // ---------------------------------------- RMD
		else if(response.find("400 Bad Request\r\n") == 0 || response.find("404 Not found\r\n") == 0){
			cerr << packet_str + i;
		}
		else{
			cerr << "unknown response from server\n"<< packet.get_str();
		}
	}
	packet.clear();
  comm_socket.Close();
  return 0;
}
