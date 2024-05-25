#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <unistd.h>

int main(int argc, char **argv)
{
    int fd;
    off_t ret;

    if (argc < 2)
    {
        fprintf(stderr, "Usage...\n");
        exit(1);
    }

    fd = open(argv[1], O_WRONLY|O_CREAT|O_TRUNC, 0666);
    if (fd < 0)
    {
        perror("open()");
        exit(1);
    }

    /*
     * 不带单位的数值是没有意义的，如果不带单位会整数溢出
     * 整形有各种各样的形式，不带单位会认为是有符号的整形，造成数据溢出
     */
    ret = lseek(fd, 5LL*1024LL*1024LL*1024LL-1LL, SEEK_SET);
    if (ret < 0)
    {
        perror("lseek()");
        exit(1);
    }

    write(fd, "", 1);

    close(fd);

    exit(0);
}
