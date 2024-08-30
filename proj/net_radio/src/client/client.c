#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>

#include <net/if.h>

#include <unistd.h>

#include "client.h"

#define DEFAULT_PLAYER_CMD "/usr/local/bin/mpg123 > /dev/null"
//#define DEFAULT_PLAYER_CMD "/usr/bin/mpg123 > /dev/null"

/* 初始化级别：命令行参数 > 环境变量 > 配置文件 > 默认值 */
struct client_conf_st client_conf
{
    .recvport = DEFAULT_RECEICE_PROT,
    .multigroup = DEFAULT_MULTI_GROUP,
    .player_cmd = DEFAULT_PLAYER_CMD,
};

static void print_help()
{
    printf("Usage: [options]\n");
    printf(" -P, --port       Specify receiving port\n");
    printf(" -M, --multigroup Specify multicast group\n");
    printf(" -p, --player     Specify player command\n");
    printf(" -H, --help       Display this help message\n");
}

int main(int argc, char *argv[])
{
    int opt;
    int opt_index = 0;

    struct option option =
    {
        {"port", required_argument, 0, 'P'},
        {"mutilgroup", required_argument, 0, 'M'},
        {"player", required_argument, 0, 'p'},
        {"help", required_argument, 0, 'H'},
        {0, 0, 0, 0},
    };

    while (1)
    {
        opt = getopt_long(argc, argv, "P:M:p:H:", &option, &opt_index);
        switch (opt)
        {
            case p:
                client_conf.recvport = optarg;
                break;
            case M:
                client_conf.mutilgroup = optarg;
                break;
            case P:
                client_conf.player_cmd = optarg;
                break;
            case H:
                print_help();
                exit(0);
                break;
            default:
                abort();
                break;
        }
    }

    int sd;

    sd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sd < 0)
    {
        perror("socket()");
        exit(1);
    }

    struct ip_mreqn optval;

    inet_pton(AF_INET, client_conf.mutilgroup, &optval.imr_multiaddr);
    inet_pton(AF_INET, "0.0.0.0", &optval.imr_address);
    optval.imr_ifindex = if_nametoindex("eth0");

    if (setsockopt(sd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &optval, sizeof(optval)) < 0)
    {
        perror("setsockopt()");
        exit(1);
    }

    int val = 1;
    if (setsockopt(sd, IPPROTO_IP, IP_MULTICAST_LOOP, &val, sizeof(val)) < 0)
    {
        perror("setsockopt()");
        exit(1);
    }

//    bind

    recvfrom

    close

    int pd[2];
    pid_t pid;

    if (pipe(pd) < 2)
    {
        perror("pipe()");
        exit(1);
    }

    pid = fork();
    if (pid < 0)
    {
        perror("fork()");
        exit(1);
    }

    if (pid == 0)
    {
        close(pd[1]);
        dup2(pd[0], 1);
        if (pd[0] > 0)
        {
            close(pd[0]);
        }

        execl("/bin/bash", "sh", "-c", client_conf.player_cmd, NULL);
        perror("execl()");
        exit(1);
    }
    else
    {
        close(pd[0]);
    }
    struct msg_list_st *msg_lst;

    msg_lst = malloc(MSG_LIST_MAX);
    if (msg_list == NULL)
    {
        perror("malloc()");
        exit(1);
    }


    while (1)
    {
        recvfrom(sd, 
    }

    exit(0);
}
