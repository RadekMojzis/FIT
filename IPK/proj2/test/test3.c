
#include <sys/ioctl.h>
#include <net/if.h> 
#include <sys/ioctl.h>
#include <net/if.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAC_LENGTH 6
#define IPV4_LENGTH 4

struct arp_packet{
	unsigned short hw_type;
	unsigned short p_type;
	unsigned char  hw_len;
	unsigned char  p_len;
	unsigned short op;
	unsigned char  sender_mac[MAC_LENGTH];
	unsigned char  sender_ip [IPV4_LENGTH];
	unsigned char  target_mac[MAC_LENGTH];
	unsigned char  target_ip [IPV4_LENGTH];
}; 


int main(){
  struct ifreq ifr;
  struct ifconf ifc;
	char buffer[1024];
	char send_buff[512];
	char recv_buff[512];
	char arp_buff[512];
	char response_buff[1024];
	
	struct ethhdr *send    = (struct ethhdr *)send_buff;
	struct ethhdr *receave = (struct ethhdr *)recv_buff;
	
	struct arp_packet *arp_req = (struct arp_header *)arp_buff;
	struct arp_packet *arp_resp = (struct arp_header *)response_buff;

  int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
  if (sock == -1) {
		perror("Socket error: ");
		return 1;
	};

  ifc.ifc_len = sizeof(buffer);
  ifc.ifc_buf = buffer;
  if (ioctl(sock, SIOCGIFCONF, &ifc) == -1) { 
		perror("Address error: ");
		return 1;
	}

  struct ifreq* it = ifc.ifc_req;
  const struct ifreq* const end = it + (ifc.ifc_len / sizeof(struct ifreq));
  unsigned char m[6];
	
	while(it != end){
    strcpy(ifr.ifr_name, it->ifr_name);
    if (ioctl(sock, SIOCGIFFLAGS, &ifr) == 0) {
      if (! (ifr.ifr_flags & IFF_LOOPBACK)) {
        if (ioctl(sock, SIOCGIFHWADDR, &ifr) == 0) {
          memcpy(m, ifr.ifr_hwaddr.sa_data, 6);
					if(!strcmp(it->ifr_name, "eth0")){
						int sock = socket(AF_INET, SOCK_DGRAM, 0);
						struct ifreq ahoj;
						ahoj.ifr_addr.sa_family = AF_INET;
						strncpy(ahoj.ifr_name, it->ifr_name, IFNAMSIZ-1);
						ioctl(sock, SIOCGIFADDR, &ahoj);
						close(sock);
						
						//mac adresa - m
						//ip adresa ((struct sockaddr_in *)&ahoj.ifr_addr)->sin_addr
					}
        }
      }
    }
    else {
			perror("sth funky is going on");
		}
		it++;
  }
}






