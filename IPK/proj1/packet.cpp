
#include "packet.hpp"
#include <string>
#include <string.h>

/*Packet::~Packet(){
	if(buffer != NULL)
		delete [] buffer;
	buffer = NULL;
}*/

char* Packet::get_str(){
	return buffer;
}

void Packet::set_str(char* str){
	if(buffer != NULL){
		delete buffer;
		buffer = NULL;
	}
	buffer = new char[strlen(str)];
	size = strlen(str);
	strncpy(buffer, str, strlen(str));
	return;
}

size_t Packet::length(){
	return size;
}