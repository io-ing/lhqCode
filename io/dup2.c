#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define FNAME "/tmp/out"

int main()
{
    int fd;

    close(1);
    fd = open(FNAME , O_WRONLY|O_CREAT|O_TRUNC, 0666);
    if (fd < 0)
    {
        perror("open()");
        exit(1);
    }

    // ---------------------------
    puts("hello");

    exit(0);
}
