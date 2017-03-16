
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

bool Packet::append(const char* str){
	int size_of_str = strlen(str);
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