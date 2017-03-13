
#ifndef MY_PACKET_DEFINITION
#define MY_PACKET_DEFINITION

#include "socket.hpp"
#include <string>
using namespace std;

class Packet{
  private:
    string buffer;
  public:
    void set(string input){buffer = input; return;};
		const char* get_str();
	  size_t length();
};

#endif