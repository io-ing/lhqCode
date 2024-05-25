#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFFSIZE 1024

int main(int argc, char *argv[])
{
    int fd_src, fd_dest;
    char buffer[BUFFSIZE] = {0};
    int len;
    
    if (argc < 3)
    {
        perror("usage...\n");
        exit(1);
    }

    fd_src = open(argv[1], O_RDONLY);
    if (fd_src < 0)
    {
        perror("open error\n");
        exit(1);
    }
    fd_dest = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, 0666);
    if (fd_dest < 0)
    {
        close(fd_src);
        perror("open error\n");
        exit(1);
    }

    while(1)
    {
        len = read(fd_src, buffer, BUFFSIZE);
        if (len < 0)
        {
            perror("read error\n");
            exit(1);
        }
        if (len == 0)
            break;
    
        // warn; len not buffsize
        write(fd_dest, buffer, len);
    }

    close(fd_dest);
    close(fd_src);

    exit(0);
}
