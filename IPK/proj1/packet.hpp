
#ifndef MY_PACKET_DEFINITION
#define MY_PACKET_DEFINITION

#include "socket.hpp"
#include <string>
using namespace std;

class Packet{
  private:
    char *buffer;
		size_t size;
  public:
	  //~Packet();
		Packet(){buffer = NULL; size = 0;};
		//void set_str(string str);
		void set_str(char* str);
    char* get_str();
	  size_t length();
};

#endif