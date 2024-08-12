#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <time.h>
#include <arpa/inet.h>

#include "proto.h"

#define IPSTRSIZE 40
#define BUFSIZE 1024

void server_job(int sd)
{
    char buf[BUFSIZE];
    int len;

    len = snprintf(buf, sizeof(buf), FMT_STAMP, (long long)time(NULL));

    if (send(sd, buf, len, 0) < 0)
    {
        perror("send()");
        exit(1);
    }
    sleep(10);
}

int main()
{
    int sd;
    int newsd;
    struct sockaddr_in laddr, raddr;
    socklen_t raddr_len;
    char ipstr[IPSTRSIZE];
    pid_t pid;

    sd = socket(AF_INET, SOCK_STREAM, 0);
    if (sd < 0)
    {
        perror("socket()");
        exit(1);
    }

    int val = 1;
    if (setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val)) < 0)
    {
        perror("setsockopt()");
        exit(1);
    }

    laddr.sin_family = AF_INET;
    laddr.sin_port = htons(atoi(SERVER_PORT));
    inet_pton(AF_INET, "0.0.0.0", &laddr.sin_addr);
    if (bind(sd, (void *)&laddr, sizeof(laddr)) < 0)
    {
        perror("bind()");
        exit(1);
    }

    if (listen(sd, 200) < 0)
    {
        perror("listen()");
        exit(1);
    }

    raddr_len = sizeof(raddr);
    while(1)
    {
        /* 需要用新的文件描述符接收，否则是假错会覆盖掉当前文件描述符 */
        newsd = accept(sd, (void *)&raddr, &raddr_len);
        if (newsd < 0)
        {
            /* 需要注意有可能出现假错，在这里暂时不考虑 */
            perror("accept()");
            exit(1);
        }

        inet_ntop(AF_INET, &raddr.sin_addr, ipstr, IPSTRSIZE);
        printf("%s:%d\n", ipstr, ntohs(raddr.sin_port));

        pid = fork();
        if (pid < 0)
        {
            perror("fork()");
            exit(1);
        }

        if (pid == 0)
        {
            close(sd);
            server_job(newsd);
            close(newsd);
            exit(0);
        }

        close(newsd);
    }


    close(sd);

    exit(0);
}