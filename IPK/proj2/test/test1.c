
#include <sys/ioctl.h>
#include <net/if.h> 
#include <sys/ioctl.h>
#include <net/if.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>


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

    for (; it != end; ++it) {
        strcpy(ifr.ifr_name, it->ifr_name);
        if (ioctl(sock, SIOCGIFFLAGS, &ifr) == 0) {
            if (! (ifr.ifr_flags & IFF_LOOPBACK)) { // don't count loopback
                if (ioctl(sock, SIOCGIFHWADDR, &ifr) == 0) {
                    success = 1;
                    break;
                }
            }
        }
        else {
					perror("sth funky is going on");
				}
    }

    unsigned char m[6];

    if (success) memcpy(m, ifr.ifr_hwaddr.sa_data, 6);
		printf("adress: %.2X.%.2X.%.2X.%.2X.%.2X.%.2X", m[0], m[1], m[2], m[3], m[4], m[5]);
}