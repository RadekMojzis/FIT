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
#include <sys/types.h>
#include <sys/stat.h>
#include <cstring>
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
				if(*port < 0){
					cerr << "Did you really just set negative port number?... Sorry, cant help you...\n";
				}
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

string execute_shell_command(string command) {
  string result = "";
  FILE * stream;
  const int max_buffer = 256;
  char buffer[max_buffer];
	
  command.append(" 2>&1");

  stream = popen(command.c_str(), "r");
  if (stream) {
    while (!feof(stream)){
      if (fgets(buffer, max_buffer, stream) != NULL) result.append(buffer);
      memset(buffer, 0, sizeof(buffer));
    }
    pclose(stream);
  }
  return result;
}

string get_path(string request){
	string path = "";
	int i = 1;
	while(request[i] != '?'){
		path += request[i];
    i++;
		if(i > request.length()){
			return "";
		}
	}
	return path;
}

int get_type(string request){
  //cout << request << "\n";
  if(request.find("?type=file HTTP/1.1") != string::npos)
	  return 0;
	if(request.find("?type=folder HTTP/1.1") != string::npos)
		return 1;
	else return 2;
}

void add_headder(Packet &packet, int length = 0){
	time_t now = time(NULL);
	char buf[128];
  struct tm tm = *gmtime(&now);
  strftime(buf, 128, "Date: %a, %d %b %Y %H:%M:%S %Z\r\n", &tm);
	packet.append(buf);
	packet.append("Content-Type: podle MIME... nevim\r\n");
	packet.append("Content-Encoding: identity\r\n");
	packet.append("Content-Length: ");
	string str_length = to_string(length);
	packet.append(str_length.c_str());
	packet.append("\r\n\r\n");
}

void execute_request(Packet &packet){
	int i = 0;
	char* packet_str = packet.get_str();
	struct stat file_status;
	string request = "";
	string path = "";
	int type = 0;
	string message = "";
	while(packet_str[i] != '\r'){
		request += packet_str[i];
		i++;
	}
	if(request.find("PUT") == 0){
		request.erase(0, 4);
		path = get_path(request);
		if(path == "") goto BAD_REQ;
		request.erase(0, path.length());
		type = get_type(request);
		if(type == 2) goto BAD_REQ;
		if(type == 0){
		  i = 0;
			while(packet_str[i] != '\r' || packet_str[i+1] != '\n' || packet_str[i+2] != '\r' || packet_str[i+3] != '\n')
				i++;
			i+=4;
      FILE *file = fopen(path.c_str(), "wb");
      if(file == NULL){
        //cerr << "things went south!\n" << path << "\n";
        goto BAD_REQ;
      }
      fwrite(packet_str + i, sizeof(char), get_filesize(packet_str), file);
      fclose(file);
			packet.set_str("200 OK\r\n");
			add_headder(packet);
			return;
		}
		if(type == 1){
			message = execute_shell_command("mkdir " + path);
			packet.set_str("200 OK\r\n");
			add_headder(packet, message.length());
			packet.append(message.c_str());
			return;
		}
	}
	else if(request.find("GET") == 0){
    //cout << "sth is going on...\n";
		request.erase(0, 4);
		path = get_path(request);
		request.erase(0, path.length());
	  type = get_type(request);
    //cout << type << "\n";
    if(type == 2) goto BAD_REQ;
		if(type == 0){
			if(stat(path.c_str(), &file_status))goto NOT_FOUND;
			if(file_status.st_mode == S_IFDIR){
				goto BAD_REQ;
			}
			FILE *file = fopen(path.c_str(), "rb");
			fseek (file , 0 , SEEK_END);
			int filesize = ftell (file);
			rewind (file);
			
			packet.set_str("200 OK\r\n");
			add_headder(packet, filesize);
			packet.append(file);
			return;
		}
		if(type == 1){
      if(stat(path.c_str(), &file_status))goto NOT_FOUND;
		  if(file_status.st_mode == S_IFREG){
				goto BAD_REQ;
			}
			message = execute_shell_command("ls " + path);
			packet.set_str("200 OK\r\n");
			add_headder(packet, message.length());
			packet.append(message.c_str());
			return;
		}
	}
	else if(request.find("DELETE") == 0){
		request.erase(0, 7);
		path = get_path(request);
		request.erase(0, path.length());
	  type = get_type(request);
		if(type == 2) goto BAD_REQ;
		if(type == 0){
			if(stat(path.c_str(), &file_status))goto NOT_FOUND;
			if(file_status.st_mode == S_IFDIR){
				goto BAD_REQ;
			}
			message = execute_shell_command("rm " + path);
			packet.set_str("200 OK\r\n");
			add_headder(packet, message.length());
			packet.append(message.c_str());
      packet.append("\0");
			return;
		}
		if(type == 1){
			if(stat(path.c_str(), &file_status))goto NOT_FOUND;
			if(file_status.st_mode == S_IFREG){
        goto BAD_REQ;
      }
			message = execute_shell_command("rmdir " + path);
			packet.set_str("200 OK\r\n");
			add_headder(packet, message.length());
			packet.append(message.c_str());
      packet.append("\0");
			return;
		}
	}
	else{
		BAD_REQ:
		packet.set_str("400 Bad Request\r\n");
		add_headder(packet);
		return;
	}
  NOT_FOUND:
		packet.set_str("404 Not found\r\n");
		add_headder(packet);
		return;

}

int main (int argc, char* argv[]){
	string root_folder = "/";
	int port = 6677;
	get_args(argc, argv, &root_folder, &port);

	
	Socket welcome_socket;
  if(welcome_socket.init()) return 1;
	
	if(welcome_socket.listen(port)) return 1;
  
  //cout << "server set, waiting for connection..\n";
  
	Socket comm_socket;
  while(1){
    if(comm_socket.Accept(welcome_socket.number()))return 1;
    if(comm_socket.number() > 0){
			Packet pack;
			Packet& packet = pack;
			
      if(!comm_socket.recv(packet)){
        int i = 0;
        char *packet_str = packet.get_str();
        string request;
        while(packet_str[i] != '\r' || packet_str[i+1] != '\n' || packet_str[i+2] != '\r' || packet_str[i+3] != '\n'){
          request += packet_str[i];
          i++;
        }
				cout << "Hello, new request is comming\n" << request << "\r\n\r\n---------------- END OF TRANSMITION ------------\n";
				execute_request(packet);
				if(comm_socket.send(packet)) return 1;
			}
	  }
	  comm_socket.Close();
  }
  
	return 0;
}
