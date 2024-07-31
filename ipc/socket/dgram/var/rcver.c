#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "proto.h"

#define IPSTRSIZE 40

int main()
{
    int size;
    int sd;
    struct sockaddr_in laddr, raddr;
    struct msg_st *rbufp;
    socklen_t raddr_len;
    char ipstr[IPSTRSIZE];

    // name[1] not name[0]
    size = sizeof(struct msg_st) + NAMEMAX - 1;
    rbufp = malloc(size);
    if (rbufp == NULL)
    {
        perror("malloc()");
        exit(1);
    }

    sd = socket(AF_INET, SOCK_DGRAM, 0/* IPPROTO_UDP */);
    if (sd < 0)
    {
        perror("socket()");
        exit(1);
    }

    laddr.sin_family = AF_INET;
    laddr.sin_port = htons(atoi(RCVPROT));
    inet_pton(AF_INET, "0.0.0.0", (void *)&laddr.sin_addr.s_addr);

    if (bind(sd, (void *)&laddr, sizeof(laddr)) < 0)
    {
        perror("bind()");
        exit(1);
    }

    /* 非常重要，否则会第一次打印地址不对，以后会对 */
    raddr_len = sizeof(raddr);
    while(1)
    {
        recvfrom(sd, (void *)rbufp, size, 0, (void *)&raddr, &raddr_len);
        inet_ntop(AF_INET, (void *)&raddr.sin_addr.s_addr, ipstr, sizeof(ipstr));
        printf("recv from ip = %s port = %d\n", ipstr, ntohs(raddr.sin_port));
        printf("name = %s\n", rbufp->name);
        printf("math = %d\n", ntohl(rbufp->math));
        printf("chinese = %d\n", ntohl(rbufp->chinese));
    }

    close(sd);

    exit(0);
}
