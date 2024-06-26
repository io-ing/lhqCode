#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <string.h>

#include "mytbf.h"

#define CPS 10
#define BUFSIZE 1024 
#define BURST 100

void main(int argc, char **argv)
{
    int fda, fdb = 1;
    int len, ret;
    char buf[BUFSIZE];
    mytbf_t *tbf;
    int size;

    if (argc != 2)
    {
        perror("Usage..");
        exit(1);
    }

    tbf = mytbf_init(CPS, BURST);
    if (tbf == NULL)
    {
        fprintf(stderr, "mytbf_init()\n");
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
        size = mytbf_fetchtoken(tbf, BUFSIZE);
        if (size < 0)
        {
            fprintf(stderr, "mytbf_fetcheoken():%s\n", strerror(-size));
            exit(1);
        }

        while ((len = read(fda, buf, size)) < 0)
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

        if (size-len > 0)
            mytbf_returntoken(tbf, size-len);

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
    mytbf_destory(tbf);

    exit(0);
}
