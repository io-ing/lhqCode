#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

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

    socket
    pipe
    fork

    exit(0);
}
