
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

int main()
{
  struct ifreq ifr;
  struct ifconf ifc;
  char buf[1024];
  int success = 0;

  int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
  if (sock == -1) {
		perror("Socket error: ");
		return 1;
	};

  ifc.ifc_len = sizeof(buf);
  ifc.ifc_buf = buf;
  if (ioctl(sock, SIOCGIFCONF, &ifc) == -1) { 
		perror("Address error: ");
		return 1;
	}

  struct ifreq* it = ifc.ifc_req;
  const struct ifreq* const end = it + (ifc.ifc_len / sizeof(struct ifreq));
  unsigned char m[6];
	printf("%-20s | %-18s | %-30s\n", "interface name", "MAC address", "IP address");	
	
  for (; it != end; ++it) {
    strcpy(ifr.ifr_name, it->ifr_name);
    if (ioctl(sock, SIOCGIFFLAGS, &ifr) == 0) {
      if (! (ifr.ifr_flags & IFF_LOOPBACK)) { // don't count loopback
        if (ioctl(sock, SIOCGIFHWADDR, &ifr) == 0) {
          memcpy(m, ifr.ifr_hwaddr.sa_data, 6);
					printf("%-23s", it->ifr_name);
					for(int i = 0; i < 6; i++){
						printf("%.2X", m[i]);
						if(i != 5)printf(".");
					}
					
					int sock = socket(AF_INET, SOCK_DGRAM, 0);
 					struct ifreq ahoj;
					ahoj.ifr_addr.sa_family = AF_INET;
					strncpy(ahoj.ifr_name, it->ifr_name, IFNAMSIZ-1);
					ioctl(sock, SIOCGIFADDR, &ahoj);
					close(sock);
					printf("%4s%-30s\n","",  inet_ntoa(((struct sockaddr_in *)&ahoj.ifr_addr)->sin_addr));
        }
      }
    }
    else {
			perror("sth funky is going on");
		}
  }
	
	
	
	
}






