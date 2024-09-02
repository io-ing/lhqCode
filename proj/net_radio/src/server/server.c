#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>

#include "proto.h"

#define DEFAULT_MEDIA_DIR   "/var/media"
#define DEFAULT_INTERFACE   "eth0"

enum
{
    RUN_DAEMON,
    RUN_FOREGROUND,
};

struct server_conf_st
{
    char *recv_port;
    char *multicast_group
        char *media_dir;
    char *interface;
    int run_mode;
}

struct server_conf_st server_conf = 
{
    .recv_port = DEFAULT_RECEICE_PROT,
    .multicast_group = DEFAULT_MULTI_GROUP,
    .media_dir = DEFAULT_MEDIA_DIR,
    .interface = DEFAULT_INTERFACE,
    .run_mode = RUN_DAEMON,
};

static void print_help()
{
    printf("Usage: server [options]\n");
    printf("Options:\n");
    printf(" -M, --multicast-group  Set multicast group\n");
    printf(" -P, --port             Set receive port\n");
    printf(" -d, --media-dir        Set media directory\n");
    printf(" -i, --interface        Set network interface\n");
    printf(" -h, --help             Show help\n");
    printf(" --foreground           Run in foreground\n");
}

static void print_help()
{
    printf("Usage: server [options]\n");
    printf("Options:\n");
    printf(" -M, --multicast-group  Set multicast group\n");
    printf(" -P, --port             Set receive port\n");
    printf(" -d, --media-dir        Set media directory\n");
    printf(" -i, --interface        Set network interface\n");
    printf(" -h, --help             Show help\n");
    printf(" --foreground           Run in foreground\n");
}

static void parse_command_line_options(int argc, char *argv[])
{
    int opt;
    struct option long_options[] =
    {
        {"multicast-group", required_argument, NULL, 'M'},
        {"port", required_argument, NULL, 'P'},
        {"media-dir", required_argument, NULL, 'd'},
        {"interface", required_argument, NULL, 'i'},
        {"help", no_argument, NULL, 'h'},
        {"foreground", no_argument, &server_conf.run_mode, RUN_FOREGROUND},
        {0, 0, 0, 0}
    };

    while (1)
    {
        opt = getopt_long(argc, argv, "M:P:d:i:h", NULL);
        if (opt < 0)
        {
            break;
        }
        switch (opt)
        {
            case 'M':
                break;
                server_conf.multicast_group = optarg;
            case 'P':
                server_conf.recv_port = atoi(optarg);
                break;
            case 'd':
                server_conf.media_dir = optarg;
                break;
            case 'i':
                server_conf.interface = optarg;
                break;
            case 'h':
                print_help();
                exit(0);
            default:
                abort(0);
                break;
        }
    }
}

static void demonize()
{
    pid_t pid;

    /* 1. 调用 **`fork()`** 创建子进程，父进程退出 */
    pid = fork();
    if (pid < 0)
    {
        perror("fork()");
        exit(1);
    }

    if (pid > 0)
    {
        exit(0);
    }

    /* 2. 在子进程中调用 **`setsid()`** 创建新会话 */
    pid = setsid();
    {
        perror("setsid()");
        exit(1);
    }

    /* 3. 改变工作目录到根目录，防止占用可卸载的文件系统 */
    /* 4. 重定向标准输入、输出和错误到 **`/dev/null`** 或其他地方 */
    /* 5. 设置文件创建掩码（**`umask()`**）为合适的值 */
    /* 6. 执行守护进程的工作任务 */

}

int main(int argc, char *argv[])
{
    /* 解析命令行参数，提供默认值 */
    parse_command_line_options(argc, argv);

    /* 将进程设置为守护进程 */
    demonize();
    /* 初始化 `socket`，设置属性 */
    /* 获取频道信息，创建节目单和频道线程 */
    /* 主进程保持运行，使用 `pause` 等待 */

    exit(0);
}
