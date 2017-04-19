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


int get_args(int argc, char *argv[], string &root_folder, int *port){
	int option;
  while((option = getopt(argc, argv, "r:p:")) != -1){
		switch(option){
			case 'r':
			  root_folder = optarg;
			  break;
			case 'p':
				*port = strtol(optarg, 0, 10);
				if(errno == ERANGE){
					cerr << "Port integer OVERFLOW... seriously?!";
					return 1;
				}
				if(*port < 0){
					cerr << "Did you really just set negative port number?... Sorry, cant help you...";
					return 1;
				}
				if(*port > 65535){
					cerr << "Maximum allowed port number is 65535, sorry.\n";
					return 1;
				}
			  break;
		  default:
			  cout << "Usage: ftrestd -r <ROOT-FOLDER> -p <PORT>";
				return 0;
			  break;
		}
	}
	return 0;
}

// Yea, this function works like magic :D really nice, got a little help from my brother but its awesome

string execute_shell_command(string command) {
  string result = "";
  FILE * stream;
  const int max_buffer = 256;
  char buffer[max_buffer];
	
  command.append(" 2>&1");

  stream = popen(command.c_str(), "r");
  if (stream) {
    while (!feof(stream)){
      if (fgets(buffer, max_buffer, stream) != NULL) result+= buffer;
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
  if(request.find("=file HTTP/1.1") != string::npos)
	  return 0;
	if(request.find("=folder HTTP/1.1") != string::npos)
		return 1;
	else return 2;
}

void execute_request(Packet &packet, string root_directory){
	int i = 0;
	int type = 0;
	char* packet_str = packet.get_str();
	struct stat file_status;
	string request = "";
	string path = root_directory;
	string message = "";
	while(packet_str[i] != '\r'){
		request += packet_str[i];
		i++;
	}
	if(request.find("PUT") == 0){
		request.erase(0, 4);
		path += get_path(request);
		if(path == ""){ packet.set_message("404 Not found\r\n", "Directory not found.\n"); return;}
		request.erase(0, path.length());
		type = get_type(request);
		if(type == 2) { packet.set_message("400 Bad Request\r\n", "Unknown type.\n"); return;}
		if(type == 0){
		  i = 0;
			while(packet_str[i] != '\r' || packet_str[i+1] != '\n' || packet_str[i+2] != '\r' || packet_str[i+3] != '\n')
				i++;
			i+=4;
			FILE *file;
			if((file = fopen(path.c_str(), "r")) != NULL){
				fclose(file);
				packet.set_message("400 Bad Request\r\n", "Already exists.\n");
				return;
			}
      file = fopen(path.c_str(), "wb");
      if(file == NULL){
        packet.set_message("500 Internal Server Error\r\n", "Unable to open binary file for writing.\n");
				return;
      }
      fwrite(packet_str + i, sizeof(char), get_filesize(packet_str), file);
      fclose(file);
			packet.set_str("200 OK\r\n");
			packet.add_headder();
			return;
		}
		if(type == 1){
			message = execute_shell_command("mkdir " + path);
			if(message.length()){
				packet.set_message("400 Bad Request\r\n", "Already exists.\n");
				return;
			}
			else{
				packet.set_message("200 OK\r\n");
        return;
			}
		}
	}
	else if(request.find("GET") == 0){
		request.erase(0, 4);
		path += get_path(request);
		request.erase(0, path.length());
	  type = get_type(request);
    if(type == 2) { packet.set_message("400 Bad Request\r\n", "Unknown type.\n"); return;}
		if(type == 0){
			if(stat(path.c_str(), &file_status)){ packet.set_message("404 Not found\r\n", "File not found.\n"); return;}
			if(S_ISDIR(file_status.st_mode)){ packet.set_message("400 Bad Request\r\n", "Not a file.\n"); return;}
			FILE *file = fopen(path.c_str(), "rb");
			fseek (file , 0 , SEEK_END);
			int filesize = ftell (file);
			rewind (file);
			
			packet.set_str("200 OK\r\n");
			packet.add_headder(filesize);
			packet.append(file);
			return;
		}
		if(type == 1){
      if(stat(path.c_str(), &file_status)){ packet.set_message("404 Not found\r\n", "Directory not found.\n"); return;}
		  if(S_ISREG(file_status.st_mode)) { packet.set_message("400 Bad Request\r\n", "Not a directory.\n"); return;}
			message = execute_shell_command("ls " + path);
			packet.set_str("200 OK\r\n");
			packet.add_headder(message.length());
			packet.append(message.c_str());
			return;
		}
	}
	else if(request.find("DELETE") == 0){
		request.erase(0, 7);
		path += get_path(request);
		request.erase(0, path.length());
	  type = get_type(request);
		if(type == 2){ packet.set_message("400 Bad Request\r\n", "Unknown type.\n"); return;}
		if(type == 0){
			if(stat(path.c_str(), &file_status)){ packet.set_message("404 Not found\r\n", "File not found.\n"); return;}
			if(S_ISDIR(file_status.st_mode)){ packet.set_message("400 Bad Request\r\n", "Not a file.\n"); return;}
			message = execute_shell_command("rm " + path);
			if(message.length()){ packet.set_message("400 Bad Request\r\n", "Not a file.\n"); return;}
			else{
				packet.set_message("200 OK\r\n");
				return;
			}
			return;
		}
		if(type == 1){
			if(stat(path.c_str(), &file_status)){ packet.set_message("404 Not found\r\n", "Directory not found.\n"); return;}
			if(S_ISREG(file_status.st_mode)) { packet.set_message("400 Bad Request\r\n", "Not a directory.\n"); return;}
			message = execute_shell_command("rmdir " + path);
			if(message.length()){ packet.set_message("400 Bad Request\r\n", "Directory not empty.\n"); return;}
			else{ 
			  packet.set_message("200 OK\r\n");
				return;
			}
			return;
		}
	}
  packet.set_message("400 Bad Request\r\n","Unknown reuquest.\n");

}

int main (int argc, char* argv[]){
	string root = "";
	string &root_directory = root;
	int port = 6677;
	pid_t server_asistent;
	if(get_args(argc, argv, root_directory, &port)) return 1;
  if(root_directory.back() != '/'){root_directory.push_back('/');}
	
	
	Socket welcome_socket;
  if(welcome_socket.init()) return 1;
	
	if(welcome_socket.listen(port)) return 1;
  
	Socket comm_socket;

	while(comm_socket.Accept(welcome_socket.number()) == false){
		server_asistent = fork();
		if(server_asistent < 0){
			cerr << "Sorry, no can do, insufficient system resources :/\n" << flush;
			exit(EXIT_FAILURE);
		}
		if(server_asistent == 0){
			//cout << "Asistent, reporting for duty!\n";
			break;
		}
	}
	
	if(comm_socket.number() > 0){
		Packet pack;
		Packet& packet = pack;
		
		if(!comm_socket.recv(packet)){
			int i = 0;
			char *packet_str = packet.get_str();
			execute_request(packet, root_directory);
			if(comm_socket.send(packet)) {
				packet.set_message("500 Internal Server Error\r\n", "Unable to open binary file for writing.\n");
				comm_socket.Close();
				pack.clear();
				exit(EXIT_FAILURE);
			}
		}
		pack.clear();
	}
	comm_socket.Close();
	if(server_asistent == 0){
		//cout << "Asistent is taking a nap...\n" << flush;
		exit(EXIT_SUCCESS);
	}
	return 0;
}
