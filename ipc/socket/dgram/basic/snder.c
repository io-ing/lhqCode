#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "proto.h"

int main(int argc, char **argv)
{
    int sd;
    struct msg_st msg;
    struct sockaddr raddr;
    socklen_t msgLen;

    sd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sd < 0)
    {
        perror("socket()");
        exit(1);
    }

    raddr.sin_family = AF_INET;
    raddr.sin_port = ntohs(atoi(RCVPROT));
    inet_pton(AF_INET, argv[1], &raddr.sin_addr.s_addr);

    // bind();
    strncpy(msg.name, "Lucy", sizeof(msg.name));
    msg.math = htonl(rand() % 100);
    msg.chinese = htonl(rand() % 100);

    msgLen = sizeof(raddr);
    if (sendto(sd, (void *)&msg, sizeof(msg), 0, &raddr, msgLen) < 0)
    {
        perror("sendto()");
        exit(1);
    }
    printf("sent to\n");

    close(sd);
    exit(0);
}
