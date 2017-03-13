
#include "packet.hpp"
#include <string>
const char* Packet::get_str(){
	return buffer.c_str();
}


size_t Packet::length(){
	return buffer.size();
}