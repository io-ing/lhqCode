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

#include <arpa/inet.h>

#include <errno.h>

#include "client.h"

#define DEFAULT_PLAYER_CMD "/usr/local/bin/mpg123 > /dev/null"
//#define DEFAULT_PLAYER_CMD "/usr/bin/mpg123 > /dev/null"

/* 初始化级别：命令行参数 > 环境变量 > 配置文件 > 默认值 */
struct client_conf_st client_conf =
{
    .recvport = DEFAULT_RECEICE_PROT,
    .mutilgroup = DEFAULT_MULTI_GROUP,
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

static ssize_t write_n(int fd, const char *buf, size_t length)
{
    ssize_t total_writen;
    ssize_t bytes_written;

    while (total_writen < length)
    {
        bytes_written = write(fd, buf+total_writen, length-total_writen);
        if (bytes_written < 0)
        {
            if (errno == EINTR)
                continue;

            return -1;
        }

        /* 理论上不应该发生，因为字节数为0表示写入完成 */
        if (bytes_written == 0)
        {
            break;
        }

        total_writen += bytes_written;
    }

    return total_writen;
}

int main(int argc, char *argv[])
{
    int opt;
    int opt_index = 0;

    struct option option[] =
    {
        {"port", required_argument, NULL, 'P'},
        {"mutilgroup", required_argument, NULL, 'M'},
        {"player", required_argument, NULL, 'p'},
        {"help", required_argument, NULL, 'H'},
        {0, 0, 0, 0},
    };

    while (1)
    {
        opt = getopt_long(argc, argv, "P:M:p:H:", option, &opt_index);
        switch (opt)
        {
            case 'p':
                client_conf.recvport = optarg;
                break;
            case 'M':
                client_conf.mutilgroup = optarg;
                break;
            case 'P':
                client_conf.player_cmd = optarg;
                break;
            case 'H':
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

    /* 接收节目单 */
    struct msg_list_st *msg_list;

    msg_list = malloc(MSG_LIST_MAX);
    if (msg_list == NULL)
    {
        perror("malloc()");
        exit(1);
    }

    struct sockaddr_in server_addr;
    socklen_t addrlen = sizeof(server_addr);
    ssize_t recv_len;

    while (1)
    {
        recv_len = recvfrom(sd, msg_list, MSG_LIST_MAX, 0, (void *)&server_addr, &addrlen);
        if (recv_len < sizeof(struct msg_list_st))
        {
            fprintf(stderr, "Message too small\n");
            continue;
        }

        if (msg_list->channel_id != LIST_CHANNEL_ID)
        {
            fprintf(stderr, "Channel ID does not match\n");
            continue;
        }
        break;
    }

    /* 打印节目单 */
    struct msg_listentry_st *pos;

    //for (pos=msg_list->entry; ; pos = pos + (ntohs)pos->len)
    for (pos=msg_list->entry; (char *)pos < (char *)msg_list+recv_len; pos = (void *)((char *)pos + ntohs(pos->len)))
    {
        fprintf(stdout, "%d %s\n", pos->channel_id, pos->description);
    }

    /* 选择频道 */
    int chosen_id;
    int ret;
    while (1)
    {
        scanf("%d\n", &chosen_id);
        if (ret != 1)
            exit(1);
    }

    /* 解析节目单 */

    /* 收频道包发送给子进程 */
    struct msg_channel_st *msg_channel;
    struct sockaddr_in remote_server;
    socklen_t remote_len = sizeof(remote_server);

    msg_channel = malloc(MSG_CHANNEL_MAX);
    if (msg_channel == NULL)
    {
        perror("malloc()");
        exit(1);
    }

    while (1)
    {
        recv_len = recvfrom(sd, msg_channel, MSG_CHANNEL_MAX, 0, (void *)&remote_server, &remote_len);
        if (recv_len < sizeof(struct msg_channel_st))
        {
            fprintf(stderr, "Message too small\n");
            continue;
        }

        if (remote_server.sin_addr.s_addr != server_addr.sin_addr.s_addr || remote_server.sin_port != server_addr.sin_port)
        {
            fprintf(stderr, "address not math.\n");
            continue;
        }

        if (msg_channel->id != chosen_id)
        {
            continue;
        }

        if (write_n(pd[1], (void *)&msg_channel->data, recv_len-sizeof(channel_id_t)) < 0)
        {
            fprintf(stderr, "write_n() error\n");
            exit(1);
        }
    }

    free(msg_channel);
    close(sd);

    exit(0);
}
