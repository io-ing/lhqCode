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
    int size;
    struct msg_st *sbufp;
    struct sockaddr_in raddr;
    socklen_t msgLen;

    if (argc < 3)
    {
        fprintf(stderr, "Usage...\n");
        exit(1);
    }

    if (strlen(argv[2]) > NAMEMAX)
    {
        fprintf(strerr, "Name is to long!\n");
        exit(1);
    }

    size = sizeof(struct msg_st) + strlen(argv[2]);
    sbufp = malloc(size);
    if (sbufp == NULL)
    {
        perror("malloc()");
        exit(1);
    }

    sd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sd < 0)
    {
        perror("socket()");
        exit(1);
    }

    // bind();

    strncpy(sbufp->name, argv[2], sizeof(argv[2]));
    sbufp->math = htonl(rand() % 100);
    sbufp->chinese = htonl(rand() % 100);

    raddr.sin_family = AF_INET;
    raddr.sin_port = ntohs(atoi(RCVPROT));
    inet_pton(AF_INET, argv[1], &raddr.sin_addr.s_addr);

    // msgLen = sizeof(raddr); why?
    if (sendto(sd, (void *)sbufp, size, 0, (void *)&raddr, sizeof(raddr)) < 0)
    {
        perror("sendto()");
        exit(1);
    }
    printf("sent to\n");

    close(sd);

    exit(0);
}
