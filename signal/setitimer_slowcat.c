#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <sys/time.h>

#define CPS 10
#define BUFSIZE CPS 

static int loop = 0;
// static volatile sig_atomic_t loop = 0;
// 保证这条指令的取值和赋值操作都是一条机器指令完成的

void alrm_handler(int sigNum)
{
    loop = 1;
    /* 注意：在笔者的环境下接收到多个信号会恢复信号的默认动作，所以需要重新注册 */
    signal(SIGALRM, alrm_handler);
//    alarm(1);
}

void main(int argc, char **argv)
{
    int fda, fdb = 1;
    int len, ret;
    char buf[BUFSIZE];
    struct itimerval itv;

    if (argc != 2)
    {
        perror("Usage..");
        exit(1);
    }

    signal(SIGALRM, alrm_handler);
//    alarm(1);
    itv.it_interval.tv_sec = 1;
    itv.it_interval.tv_usec = 0;
    itv.it_value.tv_sec = 1;
    itv.it_value.tv_usec = 0;

    if (setitimer(ITIMER_REAL, &itv, NULL) < 0)
    {
        perror("setitimer()");
        exit(1);
    }

    do
    {
        fda = open(argv[1], O_RDONLY);
        if (fda < 0)
        {
            if (errno != EINTR)
            {
                perror("open()");
                exit(1);
            }
        }
    }while(fda < 0);

    while(1)
    {
        // 判断完后 alarm 信号到来程序停在了 pause 上，无伤大雅，因为是令牌通，下次循环两次即可
        /* 注意：如果没有 pause，程序会处于忙等，浪费资源 */
        while(!loop)
            pause();
        // 使用精简指令集 token-- 可能不是由一条指令完成的
        loop = 0;

        while ((len = read(fda, buf, BUFSIZE)) < 0)
        {
            if (errno == EINTR)
                continue;
            perror("write()");
            break;
        }
        if (len == 0)
        {
            break;
        }
        // 注意是第三个参数是 len，不是 BUFSIZE
        while ((ret = write(fdb, buf, len)) < 0)
        {
            if (errno == EINTR)
                continue;
            perror("wirte()");
            exit(1);
        }
    }

    close(fda);

    exit(0);
}
