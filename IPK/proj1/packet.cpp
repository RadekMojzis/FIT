
#include "packet.hpp"
#include <iostream>
#include <string>
#include <string.h>
#include <cstdlib>
/*Packet::~Packet(){
	if(buffer != NULL)
		delete [] buffer;
	buffer = NULL;
}*/

bool Packet::set_message(string response, string message){
	if(set_str("")) return true;
	if(append(response.c_str())) return true;
	if(add_headder(message.length())) return true;
	if(append(message.c_str())) return true;
	if(append("\0")) return true;
	
	return false;
}


bool Packet::add_headder(int length){
	time_t now = time(NULL);
	char buf[128];
  struct tm tm = *gmtime(&now);
  strftime(buf, 128, "Date: %a, %d %b %Y %H:%M:%S %Z\r\n", &tm);
	if(append(buf)) return true;
	if(append("Content-Type: application/octet-stream\r\n")) return true;
	if(append("Content-Encoding: identity\r\n")) return true;
	if(append("Content-Length: "));
	string str_length = to_string(length);
	if(append(str_length.c_str())) return true;
	if(append("\r\n\r\n")) return true;
	return false;
}

char* Packet::get_str(){
	return buffer;
}

bool Packet::set_str(char* str){
	if(buffer != NULL){
		free(buffer);
		buffer = NULL;
	}
	buffer = (char*) malloc(sizeof(char)*strlen(str));
	if(buffer == NULL){cerr << "Error, memory allocation\n"; return true;}
	size = strlen(str);
	strncpy(buffer, str, strlen(str));
	return false;
}

bool Packet::append(const char* str, unsigned int size_of_str){
	char *more_room = NULL;
  more_room = (char*) realloc(buffer, size + size_of_str);
  if(more_room != NULL){
    buffer=more_room;
		memcpy(buffer + size, str, size_of_str);
		size = size + size_of_str;
	}
  else{
		free (buffer);
    cerr << "Error, memory allocation\n";
    return true;
  }	
	return false;
}

bool Packet::append(const char* str){
	unsigned int size_of_str = strlen(str);
	char *more_room = NULL;
  more_room = (char*) realloc(buffer, size + size_of_str);
  if(more_room != NULL){
    buffer=more_room;
		memcpy(buffer + size, str, size_of_str);
		size = size + size_of_str;
	}
  else{
		free (buffer);
    cerr << "Error, memory allocation\n";
    return true;
  }	
	return false;
}

int get_filesize(char *request){
	int i = 0;
	string headder;
	while(request[i] != '\r' || request[i+1] != '\n' || request[i+2] != '\r' || request[i+3] != '\n'){
		headder += request[i];
		i++;
	}
	if(headder.find("Content-Length:") == string::npos){
		return 0;
	}
	int size = strtol(request + headder.find("Content-Length:") + strlen("Content-Length:"), 0, 10);
  if(errno == ERANGE){return -1;}	
	return size;
}

bool Packet::append(FILE* file){
	fseek (file , 0 , SEEK_END);
	int filesize = ftell (file);
	rewind (file);
	char *more_room = NULL;
  more_room = (char*) realloc(buffer, size + filesize);
  if(more_room != NULL){
    buffer = more_room;
		if(fread(buffer + size, 1, filesize, file) != filesize){
			cerr << "Read error!\n";
			return true;
		}
		size = size + filesize;
	}
  else{
		free (buffer);
    cerr << "Error, memory allocation\n";
    return true;
  }	
	return false;
}



size_t Packet::length(){
	return size;
}