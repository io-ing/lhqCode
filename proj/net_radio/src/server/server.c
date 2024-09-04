#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <syslog.h>

#include <string.h>

#include <signal.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>
#include <netinet/ip.h>

#include <arpa/inet.h>

#include <net/if.h>

#include "proto.h"
#include "media_lib.h"
#include "thread_list.h"
#include "thread_channel.h"

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
    char *multicast_group;
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


static void handle_signal(int sig)
{
    syslog(LOG_INFO, "Received signal %d, exiting...", sig);
    closelog();
    exit(0);
}

static void setup_signals()
{
    struct sigaction sa;

    sa.sa_handler = demon_exit;
    sigemptyset(&sa.sa_flags);
    sa.sa_flags = 0;

    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
    sigaction(SIGQUIT, &sa, NULL);
}

static int demonize()
{
    pid_t pid;
    int fd;

    /* 1. 调用 **`fork()`** 创建子进程，父进程退出 */
    pid = fork();
    if (pid < 0)
    {
        syslog(LOG_ERR, "fork() %s", strerror(errno));
        return -1;
    }

    if (pid > 0)
    {
        exit(0);
    }

    fd = open("/dev/null", O_RDWR);
    if (fd < 0)
    {
        syslog(LOG_ERR, "open() %s", strerror(errno));
        return -1;
    }

    /* 4. 重定向标准输入、输出和错误到 **`/dev/null`** 或其他地方 */
    dup2(fd, 0);
    dup2(fd, 1);
    dup2(fd, 2);
    if (fd > 2)
    {
        close(fd);
    }

    /* 2. 在子进程中调用 **`setsid()`** 创建新会话 */
    pid = setsid();
    {
        syslog(LOG_ERR, "setsid() %s", strerror(errno));
        return -1;
    }

    /* 3. 改变工作目录到根目录，防止占用可卸载的文件系统 */
    if (chdir("/") < 0)
    {
        syslog(LOG_ERR, "chdir() %s", strerror(errno));
        return -1;
    }

    /* 5. 设置文件创建掩码（**`umask()`**）为合适的值 */
    umask(0);
}

static int socket_init()
{
    int sd;
    struct ip_mreqn mreq;

    sd = socket();
    if (sd < 0)
    {
        syslog(LOG_ERR, "socket() %s\n", strerror(errno));
        return -1;
    }

    inet_pton(AF_INET, server_conf.multicast_group, &mreq.imr_multiaddr);
    inet_pton(AF_INET, "0.0.0.0", &mreq.imr_address);
    mreq.imr_ifindex = if_indextoname(server_conf.interface);
    if (setsockopt(sd, IPPROTO_IP, IP_MULTICAST_IF, &mreq, sizeof(mreq)) < 0)
    {
        syslog(LOG_ERR, "setsockept() %s\n", strerror(errno));
        return -1;
    }

    // bind

    close(sd);
}

int main(int argc, char *argv[])
{
    openlog("netredio", LOG_PID, LOG_DAEMON|LOG_SYSLOG);

    /* 解析命令行参数，提供默认值 */
    parse_command_line_options(argc, argv);

    setup_signals();

    /* 将进程设置为守护进程 */
    if (demonize() < 0)
    {
        syslog(LOG_ERR, "demonize() error.");
        exit(1);
    }

    while (1)
    {
        pause();
    }

    /* 初始化 `socket`，设置属性 */
    socket_init();

    /* 获取频道信息，创建节目单和频道线程 */
    struct media_lib_list_entry_st *media_lib_list;
    int media_lib_size;
    int i;

    if (media_lib_get_channel_list(media_lib_list, &media_lib_size) < 0)
    {
    }

    if (thread_list_create(media_lib_list, media_lib_size) < 0)
    {
    }

    for (i=0; i<media_lib_size; i++)
    {
        if (thread_channel_create(media_lib_list, media_lib_size) < 0)
        {
        }
    }

    for (i=0; i<media_lib_size; i++)
    {
        if (thread_channel_destroy() < 0)
        {
        }
    }

    if (thread_list_destroy() < 0)
    {
    }

    if (media_lib_destroy_channel_list(media_lib_list) < 0)
    {
    }
    /* 主进程保持运行，使用 `pause` 等待 */

}
