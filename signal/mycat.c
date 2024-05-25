#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#define BUFSIZE 64

void main(int argc, char **argv)
{
    int fda, fdb = 1;
    int len, ret;
    char buf[BUFSIZE];

    if (argc != 2)
    {
        perror("Usage..");
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
        len = read(fda, buf, BUFSIZE);
        if (len < 0)
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
        ret = write(fdb, buf, len);
        if (ret < 0)
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
