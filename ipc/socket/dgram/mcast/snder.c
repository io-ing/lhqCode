#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <net/if.h>

#include "proto.h"

int main(int argc, char **argv)
{
    int sd;
    struct msg_st sbuf;
    struct sockaddr_in raddr;
    socklen_t msgLen;

    sd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sd < 0)
    {
        perror("socket()");
        exit(1);
    }

    struct ip_mreqn mreq;

    inet_pton(AF_INET, MGOUP, &mreq.imr_multiaddr);
    inet_pton(AF_INET, "0.0.0.0", &mreq.imr_address);
    mreq.imr_ifindex = if_nametoindex("eth0");

    if (setsockopt(sd, IPPROTO_IP, IP_MULTICAST_IF, &mreq, sizeof(mreq)) < 0)
    {
        perror("setsockopt()");
        exit(1);
    }

    // bind();

    strncpy(sbuf.name, "Lucy", sizeof(sbuf.name));
    sbuf.math = htonl(rand() % 100);
    sbuf.chinese = htonl(rand() % 100);

    raddr.sin_family = AF_INET;
    raddr.sin_port = ntohs(atoi(RCVPROT));
    inet_pton(AF_INET, MGOUP, &raddr.sin_addr.s_addr);

    // msgLen = sizeof(raddr); why?
    if (sendto(sd, (void *)&sbuf, sizeof(sbuf), 0, (void *)&raddr, sizeof(raddr)) < 0)
    {
        perror("sendto()");
        exit(1);
    }
    printf("sent to\n");

    close(sd);

    exit(0);
}
