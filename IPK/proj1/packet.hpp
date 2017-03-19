
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
		bool append(const char* str, unsigned int size_of_str);
		bool append(const char* str);
		bool append(FILE* file);
		bool set_str(char* str);
    char* get_str();
	  size_t length();
		bool set_message(string response, string message = "");
    bool add_headder(int length = 0);
};

int get_filesize(char *request);
#endif